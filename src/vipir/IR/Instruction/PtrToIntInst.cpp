// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/PtrToIntInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/operand/Register.h"

#include <format>

namespace vipir
{
    void PtrToIntInst::print(std::ostream& stream)
    {
        stream << std::format("%{} = ptrtoint {} -> {}", getName(mValueId), mValue->ident(), mType->getName());
    }

    std::string PtrToIntInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), getName(mValueId));
    }

    void PtrToIntInst::doConstantFold()
    {
    }

    std::vector<std::reference_wrapper<Value*> > PtrToIntInst::getOperands()
    {
        return {mValue};
    }


    void PtrToIntInst::emit(lir::Builder& builder)
    {
        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
        builder.addValue(std::make_unique<lir::Move>(vreg->clone(), mValue->getEmittedValue()));
        mEmittedValue = std::move(vreg);
    }


    PtrToIntInst::PtrToIntInst(BasicBlock* parent, Value* ptr, Type* destType)
        : Instruction(parent->getModule(), parent)
        , mValue(ptr)
        , mValueId(mModule.getNextValueId())
    {
        assert(destType->isIntegerType());
        assert(mValue->getType()->isPointerType());
        assert(mValue->getType()->getSizeInBits() == destType->getSizeInBits());

        mType = destType;
    }
}