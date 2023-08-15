// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"

#include <format>
#include <iostream>

namespace vipir
{
    Type* AllocaInst::getAllocatedType() const
    {
        return mAllocatedType;
    }

    void AllocaInst::print(std::ostream& stream) const
    {
        stream << std::format("%{} = alloca {}", mName, mAllocatedType->getName());
    }

    std::string AllocaInst::ident() const
    {
        return std::format("{}* %{}", mAllocatedType->getName(), mName); // TODO: Use proper pointer type
    }

    void AllocaInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        mEmittedValue = std::make_unique<instruction::Memory>(instruction::Register::Get("rbp"), -mStackOffset);
    }

    bool AllocaInst::requiresRegister() const
    {
        return false;
    }

    std::vector<ValueId> AllocaInst::getOperands()
    {
        return {};
    }


    AllocaInst::AllocaInst(BasicBlock* parent, ValueId id, Type* allocatedType, std::string name)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mName(name)
        , mAllocatedType(allocatedType)
    {
    }

    instruction::OperandPtr AllocaInst::getEmittedValue()
    {
        return mEmittedValue->clone();
    }
}