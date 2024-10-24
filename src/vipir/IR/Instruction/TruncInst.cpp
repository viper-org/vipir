// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/TruncInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"

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

    void TruncInst::doConstantFold()
    {
    }

    std::vector<std::reference_wrapper<Value*> > TruncInst::getOperands()
    {
        return {mValue};
    }


    void TruncInst::emit(lir::Builder& builder)
    {
        lir::OperandPtr value = mValue->getEmittedValue();
        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());

        if (dynamic_cast<lir::Immediate*>(value.get()))
        {
            mEmittedValue = value->clone();
        }
        else
        {
            builder.addValue(std::make_unique<lir::Move>(vreg->clone(), std::move(value)));

            mEmittedValue = std::move(vreg);
        }
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