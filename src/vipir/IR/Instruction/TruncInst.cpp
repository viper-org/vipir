// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/TruncInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    void TruncInst::print(std::ostream& stream)
    {
        stream << std::format("trunc {} -> {} %{}", mValue->ident(), mType->getName(), getName(mValueId));
    }

    std::string TruncInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), getName(mValueId));
    }

    std::vector<Value*> TruncInst::getOperands()
    {
        return {mValue};
    }

    
    void TruncInst::emit(MC::Builder& builder)
    {
        instruction::OperandPtr value = mValue->getEmittedValue();
        instruction::OperandPtr operand = mVReg->operand(mType->getOperandSize());

        if (auto valueReg = dynamic_cast<instruction::Register*>(value.get()))
        {
            instruction::RegisterPtr truncatedReg = std::make_unique<instruction::Register>(valueReg->getID(), mType->getOperandSize());
            if (auto operandReg = dynamic_cast<instruction::Register*>(operand.get()))
            {
                if (truncatedReg->getID() != operandReg->getID())
                {
                    builder.addValue(std::make_unique<instruction::MovInstruction>(operand->clone(), std::move(truncatedReg)));
                }
            }
        }
        else
        {
            builder.addValue(std::make_unique<instruction::MovInstruction>(operand->clone(), std::move(value)));
        }

        mEmittedValue = std::move(operand);
    }


    TruncInst::TruncInst(BasicBlock* parent, Value* ptr, Type* destType)
        : Instruction(parent->getModule(), parent)
        , mValue(ptr)
        , mValueId(mModule.getNextValueId())
    {
        assert(destType->isIntegerType());
        assert(mValue->getType()->isIntegerType());
        assert(mValue->getType()->getSizeInBits() > destType->getSizeInBits());

        mType = destType;
    }
}