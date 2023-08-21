// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/BinOpInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/MC/CmpOperand.h"

#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    void BinOpInst::print(std::ostream& stream) const
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
            case Instruction::EQ:
                operatorName = "icmp eq";
                break;
            case Instruction::NE:
                operatorName = "icmp ne";
                break;
        }
        stream << std::format("%{} = {} {}, {}", mName, operatorName, mParent->getParent()->getValue(mLeft)->ident(), mParent->getParent()->getValue(mRight)->ident());
    }

    std::string BinOpInst::ident() const
    {
        return std::format("%{}", mName);
    }

    bool BinOpInst::requiresRegister() const
    {
        return mColor != -1;
    }

    std::vector<ValueId> BinOpInst::getOperands()
    {
        return {mLeft, mRight};
    }

    void BinOpInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        switch (mOperator)
        {
            case Instruction::ADD:
            {
                instruction::OperandPtr left   = mParent->getEmittedValue(mLeft);
                instruction::OperandPtr right = mParent->getEmittedValue(mRight);
                values.push_back(std::make_unique<instruction::AddInstruction>(left->clone(), std::move(right), codegen::OperandSize::None));
                if (mColor != -1)
                {
                    values.push_back(std::make_unique<instruction::MovInstruction>(instruction::Register::Get(mRegister), std::move(left), codegen::OperandSize::None));
                    mEmittedValue = instruction::Register::Get(mRegister);
                }
                else
                {
                    mEmittedValue = std::move(left);
                }
                break;
            }
            case Instruction::SUB:
            {
                instruction::OperandPtr left   = mParent->getEmittedValue(mLeft);
                instruction::OperandPtr right = mParent->getEmittedValue(mRight);
                values.push_back(std::make_unique<instruction::SubInstruction>(left->clone(), std::move(right), codegen::OperandSize::None));
                if (mColor != -1)
                {
                    values.push_back(std::make_unique<instruction::MovInstruction>(instruction::Register::Get(mRegister), std::move(left), codegen::OperandSize::None));
                    mEmittedValue = instruction::Register::Get(mRegister);
                }
                else
                {
                    mEmittedValue = std::move(left);
                }
                break;
            }

            case Instruction::EQ:
            case Instruction::NE:
                mValues = &values;
                break;
        }
    }

    BinOpInst::BinOpInst(BasicBlock* parent, ValueId id, Value* left, Instruction::BinaryOperators op, Value* right, std::string name)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mLeft(left->getID())
        , mOperator(op)
        , mRight(right->getID())
        , mName(name)
    {
        assert(left->getType() == right->getType());
        switch (mOperator)
        {
            case Instruction::ADD:
            case Instruction::SUB:
            {
                mType = left->getType();
                break;
            }

            case Instruction::EQ:
            case Instruction::NE:
            {
                mType = Type::GetIntegerType(1);
                break;
            }
        }
    }

    instruction::OperandPtr BinOpInst::getEmittedValue()
    {
        switch (mOperator)
        {
            case Instruction::ADD:
            case Instruction::SUB:
                return std::move(mEmittedValue);

            case Instruction::EQ:
            {
                instruction::OperandPtr left   = mParent->getEmittedValue(mLeft);
                instruction::OperandPtr right = mParent->getEmittedValue(mRight);
                mValues->push_back(std::make_unique<instruction::CmpInstruction>(std::move(left), std::move(right), codegen::OperandSize::None));
                return std::make_unique<CmpOperand>(CmpOperator::EQ);
            }
            case Instruction::NE:
            {
                instruction::OperandPtr left   = mParent->getEmittedValue(mLeft);
                instruction::OperandPtr right = mParent->getEmittedValue(mRight);
                mValues->push_back(std::make_unique<instruction::CmpInstruction>(std::move(left), std::move(right), codegen::OperandSize::None));
                return std::make_unique<CmpOperand>(CmpOperator::NE);
            }
        }
    }
}