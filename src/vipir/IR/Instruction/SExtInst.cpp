// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/SExtInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    void SExtInst::print(std::ostream& stream)
    {
        stream << std::format("sext {} -> {} %{}", mValue->ident(), mType->getName(), getName(mValueId));
    }

    std::string SExtInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), getName(mValueId));
    }

    std::vector<Value*> SExtInst::getOperands()
    {
        return {mValue};
    }

    
    void SExtInst::emit(lir::Builder& builder)
    {
        lir::OperandPtr value = mValue->getEmittedValue();
        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());

        if (dynamic_cast<lir::Immediate*>(value.get()))
        {
            mEmittedValue = value->clone();
        }
        else
        {
            builder.addValue(std::make_unique<lir::MoveSX>(vreg->clone(), std::move(value)));

            mEmittedValue = std::move(vreg);
        }
    }


    SExtInst::SExtInst(BasicBlock* parent, Value* ptr, Type* destType)
        : Instruction(parent->getModule(), parent)
        , mValue(ptr)
        , mValueId(mModule.getNextValueId())
    {
        assert(destType->isIntegerType());
        assert(mValue->getType()->isIntegerType());
        assert(mValue->getType()->getSizeInBits() < destType->getSizeInBits());

        mType = destType;
    }
}