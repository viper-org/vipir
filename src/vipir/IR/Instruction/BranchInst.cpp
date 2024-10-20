// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/BranchInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/MC/CmpOperand.h"

#include "vipir/LIR/Instruction/Jump.h"

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
        return { mCondition };
    }

    bool BranchInst::isCritical()
    {
        return true;
    }


    void BranchInst::emit(lir::Builder& builder)
    {
        mParent->endPosition() = builder.getPosition();

        if (!mFalseBranch) // Unconditional branch won't have a false branch
        {
            builder.addValue(std::make_unique<lir::Jump>(mTrueBranch->getEmittedValue()));
        }
        else
        {
            mCondition->lateEmit(builder);
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
    }
}