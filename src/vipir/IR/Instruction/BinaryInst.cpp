// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/BinaryInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/MC/CmpOperand.h"
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
            
            case Instruction::EQ:
                operatorName = "cmp eq";
                break;
            case Instruction::NE:
                operatorName = "cmp ne";
                break;
            
            case Instruction::LT:
                operatorName = "cmp lt";
                break;
            case Instruction::GT:
                operatorName = "cmp gt";
                break;
            
            case Instruction::LE:
                operatorName = "cmp le";
                break;
            case Instruction::GE:
                operatorName = "cmp ge";
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
                instruction::OperandPtr left  = mLeft->getEmittedValue();
                instruction::OperandPtr right = mRight->getEmittedValue();
                builder.addValue(std::make_unique<instruction::AddInstruction>(left->clone(), std::move(right), codegen::OperandSize::None));
                mEmittedValue = std::move(left);
                break;
            }
            case Instruction::SUB:
            {
                instruction::OperandPtr left  = mLeft->getEmittedValue();
                instruction::OperandPtr right = mRight->getEmittedValue();
                builder.addValue(std::make_unique<instruction::SubInstruction>(left->clone(), std::move(right), codegen::OperandSize::None));
                mEmittedValue = std::move(left);
                break;
            }

            case Instruction::EQ:
            case Instruction::NE:
            case Instruction::LT:
            case Instruction::GT:
            case Instruction::LE:
            case Instruction::GE:
                mBuilder = &builder;
                break;
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
            case Instruction::LT:
            case Instruction::GT:
            case Instruction::LE:
            case Instruction::GE:
            {
                mType = Type::GetBooleanType();
                break;
            }
        }
    }

    instruction::OperandPtr BinaryInst::getEmittedValue()
    {
        auto GenerateCmp = [this](CmpOperator op){
            instruction::OperandPtr left  = mLeft->getEmittedValue();
            instruction::OperandPtr right = mRight->getEmittedValue();
            mBuilder->addValue(std::make_unique<instruction::CmpInstruction>(std::move(left), std::move(right), codegen::OperandSize::None));
            mEmittedValue = std::make_unique<CmpOperand>(op);
        };

        switch (mOperator)
        {
            case Instruction::EQ:
                GenerateCmp(CmpOperator::EQ);
                break;
            case Instruction::NE:
                GenerateCmp(CmpOperator::NE);
                break;

            case Instruction::LT:
                GenerateCmp(CmpOperator::LT);
                break;
            case Instruction::GT:
                GenerateCmp(CmpOperator::GT);
                break;

            case Instruction::LE:
                GenerateCmp(CmpOperator::LE);
                break;
            case Instruction::GE:
                GenerateCmp(CmpOperator::GE);
                break;

            default:
                break;
        }

        return mEmittedValue->clone();
    }
}