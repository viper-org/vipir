// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/IntToPtrInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/operand/Register.h"

#include <format>

namespace vipir
{
    void IntToPtrInst::print(std::ostream& stream)
    {
        stream << std::format("inttoptr {} -> {} %{}", mValue->ident(), mType->getName(), getName(mValueId));
    }

    std::string IntToPtrInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), getName(mValueId));
    }

    std::vector<Value*> IntToPtrInst::getOperands()
    {
        return {mValue};
    }


    void IntToPtrInst::emit(lir::Builder& builder)
    {
        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
        builder.addValue(std::make_unique<lir::Move>(vreg->clone(), mValue->getEmittedValue()));
        mEmittedValue = std::move(vreg);
    }


    IntToPtrInst::IntToPtrInst(BasicBlock* parent, Value* ptr, Type* destType)
        : Instruction(parent->getModule(), parent)
        , mValue(ptr)
        , mValueId(mModule.getNextValueId())
    {
        assert(destType->isPointerType());
        assert(mValue->getType()->isIntegerType());
        assert(mValue->getType()->getSizeInBits() == destType->getSizeInBits());

        mType = destType;
    }
}