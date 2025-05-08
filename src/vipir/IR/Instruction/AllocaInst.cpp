// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include <format>

namespace vipir
{
    void AllocaInst::print(std::ostream& stream)
    {
        stream << std::format("%{} = alloca {}", getName(mValueId), mAllocatedType->getName());
    }

    Type* AllocaInst::getAllocatedType()
    {
        return mAllocatedType;
    }

    int& AllocaInst::forceMemoryCount()
    {
        return mForceMemoryCount;
    }

    std::string AllocaInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), getName(mValueId));
    }

    void AllocaInst::doConstantFold()
    {
    }

    void AllocaInst::emit(lir::Builder& builder)
    {
        mEmittedValue = std::make_unique<lir::VirtualReg>(mVReg, mAllocatedType->getOperandSize());
    }

    AllocaInst::AllocaInst(BasicBlock* parent, Type* allocatedType)
        : Instruction(parent->getModule(), parent)
        , mAllocatedType(allocatedType)
        , mValueId(mModule.getNextValueId())
        , mForceMemoryCount(0)
    {
        mType = Type::GetPointerType(mAllocatedType);
        if (mAllocatedType->isStructType() || mAllocatedType->isArrayType()) mForceMemoryCount = 1; // Structs cannot fit inside a register
    }
}