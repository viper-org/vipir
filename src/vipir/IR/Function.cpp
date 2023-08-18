// Copyright 2023 solar-mist


#include "vipir/IR/Function.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/Module.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/singleOperandInstruction/PushInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"

#include <algorithm>
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <stack>

namespace vipir
{
    Function* Function::Create(Module& module, std::string name)
    {
        Function* func = new Function(module, std::move(name));

        module.insertGlobal(func);

        return func;
    }

    Module& Function::getModule() const
    {
        return mModule;
    }

    int& Function::getInstructionCount()
    {
        return mInstructionCount;
    }
    
    std::string_view Function::getName() const
    {
        return mName;
    }

    const std::vector<BasicBlockPtr>& Function::getBasicBlockList() const
    {
        return mBasicBlockList;
    }

    Value* Function::getValue(ValueId index) const
    {
        return mValues[index].get();
    }

    void Function::insertBasicBlock(BasicBlock* basicBlock)
    {
        mBasicBlockList.push_back(BasicBlockPtr(basicBlock));
    }

    int Function::getNumValues() const
    {
        return mValues.size();
    }

    void Function::addValue(Value* value)
    {
        mValues.push_back(std::unique_ptr<Value>(value));
    }

    void Function::print(std::ostream& stream) const
    {
        stream << std::format("define pub void @{}() {{\n   ", mName);

        for (const BasicBlockPtr& basicBlock : mBasicBlockList)
        {
            basicBlock->print(stream);
        }

        stream << "}";
    }

    std::string Function::ident() const
    {
        return std::format("@{}", mName);
    }

    bool Function::requiresRegister() const
    {
        return false;
    }

    std::vector<ValueId> Function::getOperands()
    {
        return {};
    }

    void Function::optimize(OptimizationLevel level)
    {
        std::vector<ValueId> temp;
        std::vector<decltype(mBasicBlockList.begin())> unused;
        for (auto it = mBasicBlockList.begin(); it != mBasicBlockList.end(); it++)
        {
            if ((*it)->getNoBranches() == 0)
            {
                std::copy((*it)->getInstructionList().begin(), (*it)->getInstructionList().end(), std::back_inserter(temp));
                unused.push_back(it);
            }
            else
            {
                std::copy(temp.begin(), temp.end(), (*it)->getInstructionList().end());
                temp.clear();
            }
        }

        for (auto unusedBasicBlock : unused)
        {
            mBasicBlockList.erase(unusedBasicBlock);
        }

        if (!temp.empty())
        {
            std::copy(temp.begin(), temp.end(), std::back_inserter(mValueList));
        }
    }


    void Function::emit(std::vector<instruction::ValuePtr>& values)
    {
        allocateRegisters();
        sortAllocas();

        values.emplace_back(std::make_unique<instruction::Label>(mName));

        values.emplace_back(std::make_unique<instruction::PushInstruction>(instruction::Register::Get("rbp")));
        values.emplace_back(std::make_unique<instruction::MovInstruction>(instruction::Register::Get("rbp"), instruction::Register::Get("rsp"), codegen::OperandSize::None));
        if (mTotalStackOffset)
        {
            instruction::OperandPtr offset = std::make_unique<instruction::Immediate>(mTotalStackOffset);
            values.emplace_back(std::make_unique<instruction::SubInstruction>(instruction::Register::Get("rsp"), std::move(offset), codegen::OperandSize::None));
        }

        for (const BasicBlockPtr& basicBlock : mBasicBlockList)
        {
            basicBlock->emit(values);
        }

        for (auto value : mValueList)
        {
            mValues[value]->emit(values);
        }
    }


    Function::Function(Module& module, std::string name)
        : Global(module)
        , mName(std::move(name))
        , mInstructionCount(0)
    {
    }

    void Function::sortAllocas()
    {
        std::vector<AllocaInst*> temp;

        for (const auto& basicBlock : mBasicBlockList)
        {
            for (auto instruction : basicBlock->getInstructionList())
            {
                if (AllocaInst* alloca = dynamic_cast<AllocaInst*>(mValues[instruction].get()))
                {
                    temp.push_back(alloca);
                }
            }
        }
        for (auto instruction : mValueList)
        {
            if (AllocaInst* alloca = dynamic_cast<AllocaInst*>(mValues[instruction].get()))
            {
                temp.push_back(alloca);
            }
        }

        std::sort(temp.begin(), temp.end(), [](AllocaInst* lhs, AllocaInst* rhs) {
            return lhs->getAllocatedType()->getSizeInBits() > rhs->getAllocatedType()->getSizeInBits();
        });

        int offset = 0;
        for (auto alloca : temp)
        {
            offset += alloca->getAllocatedType()->getSizeInBits() / 8;
            alloca->mStackOffset = offset;
        }
        
        mTotalStackOffset = (offset + 15) & ~15; // Align to 16 bytes
    }

    void Function::allocateRegisters()
    {
        std::vector<std::pair<ValueId, bool>> allNodes;
        std::vector<ValueId> liveNodes;

        for (const auto& basicBlock : mBasicBlockList)
        {
            for (auto instruction : basicBlock->getInstructionList())
            {
                if (mValues[instruction]->requiresRegister())
                {
                    for (ValueId id : liveNodes)
                    {
                        mValues[instruction]->mEdges.emplace_back(id, true);
                        mValues[id]->mEdges.emplace_back(instruction, true);
                    }
                    liveNodes.push_back(instruction);
                    allNodes.emplace_back(instruction, true);
                }

                for (auto operand : mValues[instruction]->getOperands())
                {
                    auto it = std::find(liveNodes.begin(), liveNodes.end(), operand);

                    if (it != liveNodes.end())
                    {
                        liveNodes.erase(it);
                    }
                }
            }
        }
        for (auto instruction : mValueList)
        {
            if (mValues[instruction]->requiresRegister())
            {
                for (ValueId id : liveNodes)
                {
                    mValues[instruction]->mEdges.emplace_back(id, true);
                    mValues[id]->mEdges.emplace_back(instruction, true);
                }
                liveNodes.push_back(instruction);
                allNodes.emplace_back(instruction, true);
            }

            for (auto operand : mValues[instruction]->getOperands())
            {
                auto it = std::find(liveNodes.begin(), liveNodes.end(), operand);

                if (it != liveNodes.end())
                {
                    liveNodes.erase(it);
                }
            }
        }

        std::stack<ValueId> stack;
        constexpr int k = 6;

        int count = allNodes.size();
        while (count)
        {
            for (auto it = allNodes.begin(); it != allNodes.end(); it++)
            {
                if (it->second && mValues[it->first]->mEdges.size() < k)
                {
                    stack.push(it->first);
                    for (auto node : allNodes)
                    {
                        // Erase nodes that share an edge
                        if (node != *it)
                        {
                            auto edge = std::find_if(mValues[node.first]->mEdges.begin(), mValues[node.first]->mEdges.end(), [this, &it](auto& node) {
                                return mValues[node.first]->mId == it->first;
                            });

                            if (edge != mValues[node.first]->mEdges.end())
                            {
                                edge->second = false;
                            }
                        }
                    }
                    it->second = false;
                    count -= 1;
                }
            }
        }

        constexpr std::array<std::array<std::string_view, 4>, 6> registers = {
            "al", "ax", "eax", "rax",
            "cl", "cx", "ecx", "rcx",
            "dl", "dx", "edx", "rdx",
            "bl", "bx", "ebx", "rbx",
            "", "si", "esi", "rsi",
            "", "di", "edi", "rdi",
        };

        constexpr std::array colors = {
            "red", "blue", "green",
            "yellow", "purple", "orange"
        };

        std::ofstream graphout("vipir.dot");
        graphout<< "\n\nstrict graph {";

        while (!stack.empty())
        {
            auto id = stack.top();
            stack.pop();

            for (auto edge : mValues[id]->mEdges)
            {
                graphout << "\n\tN" << id << " -- N" << edge.first;
            }

            int color = mValues[id]->mColor;
            if (color == -1)
            {
                color = 0;
                while (color < k)
                {
                    auto it = std::find_if(mValues[id]->mEdges.begin(), mValues[id]->mEdges.end(), [this, color](auto edge) {
                        return mValues[edge.first]->mColor == color;
                    });
                    if (it == mValues[id]->mEdges.end())
                    {
                        break;
                    }
                    color++;
                }
                mValues[id]->mColor = color;
            }
            mValues[id]->mRegister = registers[color][std::log(mValues[id]->getType()->getSizeInBits() / 8) / std::log(2)];

            graphout << "\n\tN" << id << " [color=" << colors[color] << "]";
        }

        graphout << "\n}";
    }
}