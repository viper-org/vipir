// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/IntToPtrInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Register.h"

#include <format>

namespace vipir
{
    void IntToPtrInst::print(std::ostream& stream)
    {
        stream << std::format("inttoptr {} -> {} %{}", mValue->ident(), mType->getName(), mValueId);
    }

    std::string IntToPtrInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), mValueId);
    }

    std::vector<Value*> IntToPtrInst::getOperands()
    {
        return {mValue};
    }

    
    void IntToPtrInst::emit(MC::Builder& builder)
    {
        mEmittedValue = mValue->getEmittedValue();
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