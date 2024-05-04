// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/PtrToIntInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Register.h"

#include <format>

namespace vipir
{
    void PtrToIntInst::print(std::ostream& stream)
    {
        stream << std::format("ptrtoint {} -> {} %{}", mValue->ident(), mType->getName(), getName(mValueId));
    }

    std::string PtrToIntInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), getName(mValueId));
    }

    std::vector<Value*> PtrToIntInst::getOperands()
    {
        return {mValue};
    }

    
    void PtrToIntInst::emit(MC::Builder& builder)
    {
        mEmittedValue = mValue->getEmittedValue();
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