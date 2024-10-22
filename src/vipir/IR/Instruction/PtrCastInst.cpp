// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/PtrCastInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"

#include <format>

namespace vipir
{
    void PtrCastInst::print(std::ostream& stream)
    {
        stream << std::format("ptr cast {} -> {} %{}", mPtr->ident(), mType->getName(), getName(mValueId));
    }

    std::string PtrCastInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), getName(mValueId));
    }

    std::vector<Value*> PtrCastInst::getOperands()
    {
        return {mPtr};
    }

    void PtrCastInst::doConstantFold()
    {
    }


    void PtrCastInst::emit(lir::Builder& builder)
    {
        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
        builder.addValue(std::make_unique<lir::Move>(vreg->clone(), mPtr->getEmittedValue()));
        mEmittedValue = std::move(vreg);
    }


    PtrCastInst::PtrCastInst(BasicBlock* parent, Value* ptr, Type* destType)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mValueId(mModule.getNextValueId())
    {
        assert(destType->isPointerType());
        assert(mPtr->getType()->isPointerType());
        mType = destType;
    }
}