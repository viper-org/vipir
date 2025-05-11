// Copyright 2025 solar-mist

#include "vipir/IR/Instruction/SelectInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/LIR/Instruction/Compare.h"
#include "vipir/LIR/Instruction/Jump.h"
#include "vipir/LIR/Instruction/Move.h"

#include "vipir/LIR/Label.h"

#include "vipir/Module.h"

#include <cassert>

namespace vipir
{
    void SelectInst::print(std::ostream& stream)
    {
        stream << std::format("%{} = select {}, {}, {}", getName(mValueId), mCondition->ident(), mTrue->ident(), mFalse->ident());
    }

    std::string SelectInst::ident() const
    {
        return std::format("%{}", getName(mValueId));
    }

    void SelectInst::doConstantFold()
    {
        if (mCondition->isConstantFolded())
        {
            bool condition = mCondition->getConstantFoldedValue();
            if (condition && mTrue->isConstantFolded())
            {
                mConstantFoldedValue = mTrue->getConstantFoldedValue();
                mIsConstantFolded = true;
            }
            else if (!condition && mFalse->isConstantFolded())
            {
                mConstantFoldedValue = mFalse->getConstantFoldedValue();
                mIsConstantFolded = true;
            }
        }
    }

    std::vector<std::reference_wrapper<Value*> > SelectInst::getOperands()
    {
        return Value::getOperands();
    }

    std::vector<int> SelectInst::getRegisterSmashes()
    {
        return Value::getRegisterSmashes();
    }

    void SelectInst::emit(lir::Builder& builder)
    {
        if (mIsConstantFolded)
        {
            mEmittedValue = std::make_unique<lir::Immediate>(mConstantFoldedValue);
            return;
        }
        else if (mCondition->isConstantFolded()) // if the condition is constant folded but the values aren't, we can just set emitted value to true/false emitted value
        {
            bool condition = mCondition->getConstantFoldedValue();
            if (condition)
            {
                mEmittedValue = mTrue->getEmittedValue();
            }
            else
            {
                mEmittedValue = mFalse->getEmittedValue();
            }

            return;
        }

        auto condition = mCondition->getEmittedValue();
        if (auto imm = dynamic_cast<lir::Immediate*>(condition.get()))
        {
            if (imm->value())
            {
                mEmittedValue = mTrue->getEmittedValue();
            }
            else
            {
                mEmittedValue = mFalse->getEmittedValue();
            }
            return;
        }

        static uintmax_t selectLabelIndex = 0;

        // TODO: https://www.felixcloutier.com/x86/cmovcc
        std::string trueLabelName = std::format(".select.{}", selectLabelIndex++);
        std::string endLabelName = std::format(".select.{}", selectLabelIndex++);

        auto trueLabel = std::make_unique<lir::Lbl>(trueLabelName, false);
        auto endLabel = std::make_unique<lir::Lbl>(endLabelName, false);
        auto vreg = std::make_unique<lir::VirtualReg>(mVReg, mTrue->getType()->getOperandSize());

        builder.addValue(std::make_unique<lir::Compare>(std::move(condition), lir::CMP::Operator::NE, std::make_unique<lir::Immediate>(0)));
        condition = std::make_unique<lir::CMP>(lir::CMP::Operator::NE);

        builder.addValue(std::make_unique<lir::CondJump>(std::move(trueLabel), std::move(condition)));

        builder.addValue(std::make_unique<lir::Move>(vreg->clone(), mFalse->getEmittedValue()));
        builder.addValue(std::make_unique<lir::Jump>(std::move(endLabel)));

        builder.addValue(std::make_unique<lir::Label>(std::move(trueLabelName), false));
        builder.addValue(std::make_unique<lir::Move>(vreg->clone(), mTrue->getEmittedValue()));

        builder.addValue(std::make_unique<lir::Label>(std::move(endLabelName), false));

        mEmittedValue = std::move(vreg);
    }

    SelectInst::SelectInst(BasicBlock* parent, Value* condition, Value* trueValue, Value* falseValue)
        : Instruction(parent->getModule(), parent)
        , mCondition(condition)
        , mTrue(trueValue)
        , mFalse(falseValue)
        , mValueId(mModule.getNextValueId())
    {
        assert(condition->getType()->isBooleanType());
        assert(trueValue->getType() == falseValue->getType());

        mType = trueValue->getType();
    }
}