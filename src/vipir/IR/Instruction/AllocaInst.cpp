// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"

#include <format>

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
        return std::format("{}* {}", mAllocatedType->getName(), mName); // TODO: Use proper pointer type
    }

    instruction::OperandPtr AllocaInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        return std::make_unique<instruction::Memory>(instruction::Register::Get("rbp"), mStackOffset);
    }

    AllocaInst::AllocaInst(BasicBlock* parent, Type* allocatedType, std::string name)
        : Instruction(parent->getParent()->getModule())
        , mName(name)
        , mAllocatedType(allocatedType)
    {
    }
}