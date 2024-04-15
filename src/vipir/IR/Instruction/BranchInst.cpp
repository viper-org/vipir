// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/BranchInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/MC/CmpOperand.h"

#include "vasm/instruction/singleOperandInstruction/JmpInstruction.h"
#include "vasm/instruction/singleOperandInstruction/JccInstruction.h"

#include <cassert>
#include <format>
#include <iostream>

namespace vipir
{
    void BranchInst::print(std::ostream& stream)
    {
        if (!mFalseBranch) // Unconditional branch won't have a false branch
        {
            stream << std::format("branch {}", mTrueBranch->ident());
        }
        else
        {
            stream << std::format("branch if {}, {}, {}", mCondition->ident(), mTrueBranch->ident(), mFalseBranch->ident());
        }
    }

    std::string BranchInst::ident() const
    {
        return "%undef";
    }

    std::vector<Value*> BranchInst::getOperands()
    {
        return {};
    }


    void BranchInst::emit(MC::Builder& builder)
    {
        if (!mFalseBranch) // Unconditional branch won't have a false branch
        {
            builder.addValue(std::make_unique<instruction::JmpInstruction>(std::move(mTrueBranch->getEmittedValue())));
        }
        else
        {
            instruction::OperandPtr& condition = mCondition->getEmittedValue();
            CmpOperand* cmp = static_cast<CmpOperand*>(condition.get());
            switch (cmp->getOperator())
            {
                case CmpOperator::EQ:
                {
                    builder.addValue(std::make_unique<instruction::JeInstruction>(std::move(mTrueBranch->getEmittedValue())));
                    builder.addValue(std::make_unique<instruction::JmpInstruction>(std::move(mFalseBranch->getEmittedValue())));
                    break;
                }
                case CmpOperator::NE:
                {
                    builder.addValue(std::make_unique<instruction::JneInstruction>(std::move(mTrueBranch->getEmittedValue())));
                    builder.addValue(std::make_unique<instruction::JmpInstruction>(std::move(mFalseBranch->getEmittedValue())));
                    break;
                }

                case CmpOperator::LT:
                {
                    builder.addValue(std::make_unique<instruction::JlInstruction>(std::move(mTrueBranch->getEmittedValue())));
                    builder.addValue(std::make_unique<instruction::JmpInstruction>(std::move(mFalseBranch->getEmittedValue())));
                    break;
                }
                case CmpOperator::GT:
                {
                    builder.addValue(std::make_unique<instruction::JgInstruction>(std::move(mTrueBranch->getEmittedValue())));
                    builder.addValue(std::make_unique<instruction::JmpInstruction>(std::move(mFalseBranch->getEmittedValue())));
                    break;
                }

                case CmpOperator::LE:
                {
                    builder.addValue(std::make_unique<instruction::JleInstruction>(std::move(mTrueBranch->getEmittedValue())));
                    builder.addValue(std::make_unique<instruction::JmpInstruction>(std::move(mFalseBranch->getEmittedValue())));
                    break;
                }
                case CmpOperator::GE:
                {
                    builder.addValue(std::make_unique<instruction::JgeInstruction>(std::move(mTrueBranch->getEmittedValue())));
                    builder.addValue(std::make_unique<instruction::JmpInstruction>(std::move(mFalseBranch->getEmittedValue())));
                    break;
                }
            }
        }
    }


    BranchInst::BranchInst(BasicBlock* parent, BasicBlock* destination)
        : Instruction(parent->getParent()->getModule(), parent)
        , mCondition(nullptr)
        , mTrueBranch(destination)
        , mFalseBranch(nullptr)
    {
    }

    BranchInst::BranchInst(BasicBlock* parent, Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch)
        : Instruction(parent->getParent()->getModule(), parent)
        , mCondition(condition)
        , mTrueBranch(trueBranch)
        , mFalseBranch(falseBranch)
    {
        assert(mCondition->getType() == Type::GetBooleanType());
    }
}