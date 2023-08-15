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
#include <queue>

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
    }


    Function::Function(Module& module, std::string name)
        : Global(module)
        , mName(std::move(name))
        , mInstructionCount(0)
    {
        // TODO: Add dil and sil
        mRegisters.push_back({"al", "ax", "eax", "rax"});
        mRegisters.push_back({"cl", "cx", "ecx", "rcx"});
        mRegisters.push_back({"dl", "dx", "edx", "rdx"});
        mRegisters.push_back({"bl", "bx", "ebx", "rbx"});
        mRegisters.push_back({"asdasdasd", "si", "esi", "rsi"});
        mRegisters.push_back({"asdadasd", "di", "edi", "rdi"});
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

    static inline std::array<std::string_view, 4> GetAllRegisterNames(std::string_view regName)
    {
        constexpr std::array<std::array<std::string_view, 4>, 6> registers = {
            "", "di", "edi", "rdi",
            "", "si", "esi", "rsi",
            "bl", "bx", "ebx", "rbx",
            "dl", "dx", "edx", "rdx",
            "cl", "cx", "ecx", "rcx",
            "al", "ax", "eax", "rax",
        };

        auto it = std::find_if(registers.begin(), registers.end(), [&regName](const auto& reg) {
            return std::find_if(reg.begin(), reg.end(), [&regName](const auto& innerReg) {
                return innerReg == regName;
            }) != reg.end();
        });

        return *it;
    }

    void Function::allocateRegisters()
    {
        std::vector<ValueId> temp;

        for (const auto& basicBlock : mBasicBlockList)
        {
            for (auto instruction : basicBlock->getInstructionList())
            {
                if (mValues[instruction]->requiresRegister())
                {
                    const auto reg = mRegisters.front();
                    mRegisters.pop_front();

                    auto regName = reg[std::log(mValues[instruction]->getType()->getSizeInBits() / 8) / std::log(2)]; // log2 gives us the index we want
                    mValues[instruction]->setRegister(regName.data());

                    temp.push_back(instruction); // Save it so we can pop the register later
                }
                for (auto operand : mValues[instruction]->getOperands())
                {
                    auto it = std::find(temp.begin(), temp.end(), operand);

                    if (it != temp.end())
                    {
                        mRegisters.push_front(GetAllRegisterNames(mValues[*it]->mRegister)); // Restore all the registers from the operands as we no longer need them
                    }
                }
            }
        }
    }
}