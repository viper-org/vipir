// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/BranchInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/MC/CmpOperand.h"

#include "vasm/instruction/singleOperandInstruction/JmpInstruction.h"
#include "vasm/instruction/singleOperandInstruction/JccInstruction.h"

#include <format>
#include <iostream>

namespace vipir
{
    void BranchInst::print(std::ostream& stream) const
    {
        if (!mFalseBranch) // Unconditional branch won't have a false branch
        {
            stream << std::format("br {}", mTrueBranch->ident());
        }
        else
        {
            stream << std::format("br {}, {}, {}", mParent->getParent()->getValue(mCondition)->ident(), mTrueBranch->ident(), mFalseBranch->ident());
        }
    }

    std::string BranchInst::ident() const
    {
        return "%undef";
    }

    bool BranchInst::requiresRegister() const
    {
        return false;
    }

    std::vector<ValueId> BranchInst::getOperands()
    {
        return {};
    }


    void BranchInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        if (!mFalseBranch) // Unconditional branch won't have a false branch
        {
            values.emplace_back(std::make_unique<instruction::JmpInstruction>(mParent->getParent()->getBasicBlockEmittedValue(mTrueBranch)));
        }
        else
        {
            instruction::OperandPtr condition = mParent->getEmittedValue(mCondition);
            CmpOperand* cmp = static_cast<CmpOperand*>(condition.get());
            switch (cmp->getOperator())
            {
                case CmpOperator::EQ:
                {
                    values.emplace_back(std::make_unique<instruction::JeInstruction>(mParent->getParent()->getBasicBlockEmittedValue(mTrueBranch)));
                    values.emplace_back(std::make_unique<instruction::JmpInstruction>(mParent->getParent()->getBasicBlockEmittedValue(mFalseBranch)));
                    break;
                }
                case CmpOperator::NE:
                {
                    values.emplace_back(std::make_unique<instruction::JneInstruction>(mParent->getParent()->getBasicBlockEmittedValue(mTrueBranch)));
                    values.emplace_back(std::make_unique<instruction::JmpInstruction>(mParent->getParent()->getBasicBlockEmittedValue(mFalseBranch)));
                    break;
                }
            }
        }
    }


    BranchInst::BranchInst(BasicBlock* parent, ValueId id, BasicBlock* destination)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mCondition(-1)
        , mTrueBranch(destination)
        , mFalseBranch(nullptr)
    {
    }

    BranchInst::BranchInst(BasicBlock* parent, ValueId id, Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mCondition(condition->getID())
        , mTrueBranch(trueBranch)
        , mFalseBranch(falseBranch)
    {
    }
}