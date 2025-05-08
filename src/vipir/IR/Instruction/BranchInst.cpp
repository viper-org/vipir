// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/BranchInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/MC/CmpOperand.h"

#include "vipir/LIR/Instruction/Jump.h"

#include "vasm/instruction/singleOperandInstruction/JmpInstruction.h"
#include "vasm/instruction/singleOperandInstruction/JccInstruction.h"

#include <algorithm>
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
            stream << std::format("branch if {} ? {} : {}", mCondition->ident(), mTrueBranch->ident(), mFalseBranch->ident());
        }
    }

    std::string BranchInst::ident() const
    {
        return "%undef";
    }

    std::vector<std::reference_wrapper<Value*> > BranchInst::getOperands()
    {
        if (mCondition) return { mCondition };
        return {};
    }

    void BranchInst::doConstantFold()
    {
        if (mCondition && mCondition->isConstantFolded())
        {
            auto value = mCondition->getConstantFoldedValue();
            if (value)
            {
                std::erase(mFalseBranch->predecessors(), mParent);
                std::erase(mParent->successors(), mFalseBranch);
                mFalseBranch = nullptr;
            }
            else
            {
                std::erase(mTrueBranch->predecessors(), mParent);
                std::erase(mParent->successors(), mTrueBranch);
                mTrueBranch = nullptr;
            }
            mIsConstantFolded = true;
        }
    }

    bool BranchInst::isCritical()
    {
        return true;
    }


    void BranchInst::emit(lir::Builder& builder)
    {
        assert(!mParent->endNode());
        mParent->endNode() = builder.getLastNode();

        if (mIsConstantFolded && !mTrueBranch)
        {
            builder.addValue(std::make_unique<lir::Jump>(mFalseBranch->getEmittedValue()));
        }

        if (!mFalseBranch) // Unconditional/constant folded branch won't have a false branch
        {
            builder.addValue(std::make_unique<lir::Jump>(mTrueBranch->getEmittedValue()));
        }
        else
        {
            mCondition->lateEmit(builder);
            auto condition = mCondition->getEmittedValue();
            if (auto imm = dynamic_cast<lir::Immediate*>(condition.get()))
            {
                if (imm->value())
                {
                    builder.addValue(std::make_unique<lir::Jump>(mTrueBranch->getEmittedValue()));
                }
                else
                {
                    builder.addValue(std::make_unique<lir::Jump>(mFalseBranch->getEmittedValue()));
                }
                return;
            }
            builder.addValue(std::make_unique<lir::CondJump>(mTrueBranch->getEmittedValue(), mCondition->getEmittedValue()));
            builder.addValue(std::make_unique<lir::Jump>(mFalseBranch->getEmittedValue()));
        }
    }


    BranchInst::BranchInst(BasicBlock* parent, BasicBlock* destination)
        : Instruction(parent->getParent()->getModule(), parent)
        , mCondition(nullptr)
        , mTrueBranch(destination)
        , mFalseBranch(nullptr)
    {
        mRequiresVReg = false;
        parent->successors().push_back(destination);
        destination->predecessors().push_back(parent);
    }

    BranchInst::BranchInst(BasicBlock* parent, Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch)
        : Instruction(parent->getParent()->getModule(), parent)
        , mCondition(condition)
        , mTrueBranch(trueBranch)
        , mFalseBranch(falseBranch)
    {
        assert(mCondition->getType() == Type::GetBooleanType());
        mRequiresVReg = false;
        parent->successors().push_back(trueBranch);
        parent->successors().push_back(falseBranch);
        trueBranch->predecessors().push_back(parent);
        falseBranch->predecessors().push_back(parent);
    }
}