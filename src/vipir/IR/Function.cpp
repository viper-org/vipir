// Copyright 2024 solar-mist

#include "vipir/IR/Function.h"

#include "vasm/instruction/operand/Immediate.h"
#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/Module.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Label.h"

#include "vasm/instruction/singleOperandInstruction/PushInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"

#include <algorithm>
#include <deque>
#include <format>

namespace vipir
{
    Function* Function::Create(FunctionType* type, Module& module, std::string_view name)
    {
        Function* func = new Function(type, module, name);

        module.insertGlobal(func);

        return func;
    }

    FunctionType* Function::getFunctionType() const
    {
        return static_cast<FunctionType*>(mType);
    }

    void Function::insertBasicBlock(BasicBlock* basicBlock)
    {
        mBasicBlockList.push_back(BasicBlockPtr(basicBlock));
    }

    void Function::print(std::ostream& stream)
    {
        stream << std::format("\n\nfunction @{}() -> {} {{\n", mName, getFunctionType()->getReturnType()->getName());
        for (auto& basicBlock : mBasicBlockList)
        {
            basicBlock->print(stream);
        }
        stream << "}";
    }

    std::string Function::ident() const
    {
        return "@" + mName;
    }

    void Function::emit(MC::Builder& builder)
    {
        mEmittedValue = std::make_unique<instruction::LabelOperand>(mName);

        allocateRegisters();
        setLocalStackOffsets();

        builder.addValue(std::make_unique<instruction::Label>(mName));

        builder.addValue(std::make_unique<instruction::PushInstruction>(instruction::Register::Get("rbp")));
        builder.addValue(std::make_unique<instruction::MovInstruction>(instruction::Register::Get("rbp"), instruction::Register::Get("rsp")));
        if (mTotalStackOffset > 0) // There are local variables
        {
            instruction::OperandPtr reg = instruction::Register::Get("rsp");
            instruction::OperandPtr stackOffset = std::make_unique<instruction::Immediate>(mTotalStackOffset);
            builder.addValue(std::make_unique<instruction::SubInstruction>(std::move(reg), std::move(stackOffset)));
        }

        for (auto& basicBlock : mBasicBlockList)
        {
            basicBlock->setEmittedValue();
        }

        for (auto& basicBlock : mBasicBlockList)
        {
            basicBlock->emit(builder);
        }
    }

    Function::Function(FunctionType* type, Module& module, std::string_view name)
        : Global(module)
        , mName(name)
        , mTotalStackOffset(0)
    {
        mType = type;
    }

    void Function::insertAlloca(AllocaInst* alloca)
    {
        mAllocaList.push_back(alloca);
    }

    void Function::setLocalStackOffsets()
    {
        std::sort(mAllocaList.begin(), mAllocaList.end(), [](AllocaInst* lhs, AllocaInst* rhs) {
            return lhs->getAllocatedType()->getSizeInBits() > rhs->getAllocatedType()->getSizeInBits();
        });

        int offset = 0;
        for (auto alloca : mAllocaList)
        {
            offset += alloca->getAllocatedType()->getSizeInBits() / 8;
            alloca->mStackOffset = offset;
        }
        
        mTotalStackOffset = (offset + 15) & ~15; // Align to 16 bytes
    }


    void Function::setLiveIntervals()
    {
        int index = 0;
        for (auto& basicBlock : mBasicBlockList)
        {
            for (auto& value : basicBlock->mValueList)
            {
                value->mInterval.first = index;
                index++;
            }
        }

        for (auto bb = mBasicBlockList.rbegin(); bb != mBasicBlockList.rend(); ++bb)
        {
            for (auto it = (*bb)->mValueList.rbegin(); it != (*bb)->mValueList.rend(); ++it)
            {
                auto& value = *it;
                for (auto operand : value->getOperands())
                {
                    // If we haven't set the last usage yet, this is it
                    if (operand->mInterval.second == -1)
                    {
                        operand->mInterval.second = index;
                    }
                }
                index--;
            }
        }

        // If any values don't have an end interval, set it to the same as the
        // start - they are never used
        for (auto& basicBlock : mBasicBlockList)
        {
            for (auto& value : basicBlock->mValueList)
            {
                if (value->mInterval.second == -1)
                {
                    value->mInterval.second = value->mInterval.first;
                }
            }
        }
    }

    std::vector<Function::Overlap> Function::getOverlaps()
    {
        std::vector<Overlap> overlaps;

        // Find all values that require a specific register
        for (auto& basicBlock : mBasicBlockList)
        {
            for (auto& value : basicBlock->mValueList)
            {
                if (value->mRegisterSmashes.size())
                {
                    overlaps.push_back(Overlap{value.get(), -1, value->mInterval.first});
                }
            }
        }


        // Set the start of the lifetime of these values
        for (auto& overlap : overlaps)
        {
            for (auto& basicBlock : mBasicBlockList)
            {
                for (auto& value : basicBlock->mValueList)
                {
                    // Check if the value's last use is before this value's first and that this value's last use is after the current lifetime beginning
                    if (value->mInterval.second <= overlap.value->mInterval.first && value->mInterval.second > overlap.start)
                    {
                        overlap.start = value->mInterval.second;
                    }
                }
            }
        }

        return overlaps;
    }

    void Function::allocateRegisters()
    {
        setLiveIntervals();

        auto overlaps = getOverlaps();

        std::deque<int> registerIDs { 0, 1, 2, 3, 4, 5 };
        std::vector<Value*> activeValues;

        auto ExpireOldIntervals = [&activeValues, &registerIDs](int i){
            std::erase_if(activeValues, [i, &registerIDs](Value* value){
                if (value->mInterval.second <= i)
                {
                    registerIDs.push_front(value->mRegisterID);
                    return true;
                }
                return false;
            });
        };

        int index = 0;
        for (auto& basicBlock : mBasicBlockList)
        {
            for (auto& value : basicBlock->mValueList)
            {
                ExpireOldIntervals(value->mInterval.first);

                for (auto& overlap : overlaps)
                {
                    if (overlap.start == index)
                    {
                        // We found a value that smashes a register so we don't want to allocate it until after
                        registerIDs.erase(std::remove_if(registerIDs.begin(), registerIDs.end(), [&overlap](int id){
                            return std::find(overlap.value->mRegisterSmashes.begin(), overlap.value->mRegisterSmashes.end(), id) != overlap.value->mRegisterSmashes.end();
                        }), registerIDs.end());
                    }
                    if (overlap.end == index)
                    {
                        // This value's smashed registers have been smashed already so we can allocate them again
                        std::copy(overlap.value->mRegisterSmashes.begin(), overlap.value->mRegisterSmashes.end(), std::front_inserter(registerIDs));
                    }
                }

                if (value->requiresRegister)
                {
                    activeValues.push_back(value.get());

                    value->mRegisterID = registerIDs.front();
                    registerIDs.pop_front();
                }
                index++;
            }
        }
    }
}