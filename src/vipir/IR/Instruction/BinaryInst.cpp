// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/BinaryInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    void BinaryInst::print(std::ostream& stream)
    {
        std::string operatorName;
        switch (mOperator)
        {
            case Instruction::ADD:
                operatorName = "add";
                break;
            case Instruction::SUB:
                operatorName = "sub";
                break;
        }
        stream << std::format("{} %{}, {}, {}", operatorName, mValueId, mLeft->ident(), mRight->ident());
    }

    std::string BinaryInst::ident() const
    {
        return std::format("%{}", mValueId);
    }

    std::vector<Value*> BinaryInst::getOperands()
    {
        return {mLeft, mRight};
    }

    void BinaryInst::emit(MC::Builder& builder)
    {
        switch (mOperator)
        {
            case Instruction::ADD:
            {
                instruction::OperandPtr& left  = mLeft->getEmittedValue();
                instruction::OperandPtr& right = mRight->getEmittedValue();
                builder.addValue(std::make_unique<instruction::AddInstruction>(left->clone(), std::move(right), codegen::OperandSize::None));
                mEmittedValue = std::move(left);
                break;
            }
            case Instruction::SUB:
            {
                instruction::OperandPtr& left  = mLeft->getEmittedValue();
                instruction::OperandPtr& right = mRight->getEmittedValue();
                builder.addValue(std::make_unique<instruction::SubInstruction>(left->clone(), std::move(right), codegen::OperandSize::None));
                mEmittedValue = std::move(left);
                break;
            }
        }
    }

    BinaryInst::BinaryInst(BasicBlock* parent, Value* left, Instruction::BinaryOperators op, Value* right)
        : Instruction(parent->getParent()->getModule(), parent)
        , mLeft(left)
        , mOperator(op)
        , mRight(right)
        , mValueId(mModule.getNextValueId())
    {
        assert(left->getType() == right->getType());
        mType = left->getType();
    }

    instruction::OperandPtr& BinaryInst::getEmittedValue()
    {
        return mEmittedValue;
    }
}