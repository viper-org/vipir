// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/UnaryInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vasm/instruction/singleOperandInstruction/Grp4Instruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    void UnaryInst::print(std::ostream& stream)
    {
        std::string operatorName;
        switch (mOperator)
        {
            case Instruction::NEG:
                operatorName = "neg";
                break;
        }
        stream << std::format("{} %{}, {}", operatorName, mValueId, mOperand->ident());
    }

    std::string UnaryInst::ident() const
    {
        return std::format("%{}", mValueId);
    }

    std::vector<Value*> UnaryInst::getOperands()
    {
        return {mOperand};
    }

    void UnaryInst::emit(MC::Builder& builder)
    {
        switch (mOperator)
        {
            case Instruction::NEG:
            {
                instruction::OperandPtr operand = mOperand->getEmittedValue();
                builder.addValue(std::make_unique<instruction::NegInstruction>(operand->clone()));
                mEmittedValue = std::move(operand);
                break;
            }
        }
    }

    UnaryInst::UnaryInst(BasicBlock* parent, Value* operand, Instruction::UnaryOperators op)
        : Instruction(parent->getParent()->getModule(), parent)
        , mOperand(operand)
        , mOperator(op)
        , mValueId(mModule.getNextValueId())
    {
        switch (mOperator)
        {
            case Instruction::NEG:
                mType = mOperand->getType();
        }
    }
}