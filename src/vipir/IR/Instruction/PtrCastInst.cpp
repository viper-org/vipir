// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/PtrCastInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include <format>

namespace vipir
{
    void PtrCastInst::print(std::ostream& stream)
    {
        stream << std::format("ptr cast {} -> {} %{}", mPtr->ident(), mType->getName(), mValueId);
    }

    std::string PtrCastInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), mValueId);
    }

    std::vector<Value*> PtrCastInst::getOperands()
    {
        return {mPtr};
    }

    
    void PtrCastInst::emit(MC::Builder& builder)
    {
        mEmittedValue = mPtr->getEmittedValue();
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