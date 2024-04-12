// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/StoreInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vasm/instruction/operand/Register.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include <format>

namespace vipir
{
    void StoreInst::print(std::ostream& stream)
    {
        stream << std::format("store {} -> {}", mValue->ident(), mPtr->ident());
    }

    std::string StoreInst::ident() const
    {
        return "%undef";
    }

    void StoreInst::emit(MC::Builder& builder)
    {
        instruction::OperandPtr& ptr   = mPtr->getEmittedValue();
        instruction::OperandPtr& value = mValue->getEmittedValue();

        builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(ptr), std::move(value), mValue->getType()->getOperandSize()));
    }

    StoreInst::StoreInst(BasicBlock* parent, Value* ptr, Value* value)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mValue(value)
    {
    }
}