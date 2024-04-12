// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"

#include <format>

namespace vipir
{
    void AllocaInst::print(std::ostream& stream)
    {
        stream << std::format("alloca {}: {}", mName, mAllocatedType->getName());
    }

    Type* AllocaInst::getAllocatedType()
    {
        return mAllocatedType;
    }

    std::string AllocaInst::ident() const
    {
        return std::format("{}* {}", mAllocatedType->getName(), mName);
    }

    void AllocaInst::emit(MC::Builder& builder)
    {
        auto base = instruction::Register::Get("rbp");
        mEmittedValue = std::make_unique<instruction::Memory>(std::move(base), -mStackOffset);
    }

    AllocaInst::AllocaInst(BasicBlock* parent, Type* allocatedType, std::string_view name)
        : Instruction(parent->getModule(), parent)
        , mAllocatedType(allocatedType)
        , mName(name)
        , mStackOffset(0)
    {
    }
}