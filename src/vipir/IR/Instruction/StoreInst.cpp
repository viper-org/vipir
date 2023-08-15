// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    void StoreInst::print(std::ostream& stream) const
    {
        stream << std::format("store {}, {}", mPtr->ident(), mValue->ident());
    }

    std::string StoreInst::ident() const
    {
        return "%undef";
    }

    instruction::OperandPtr StoreInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        instruction::OperandPtr ptrOperand   = mPtr->emit(values);
        instruction::OperandPtr valueOperand = mValue->emit(values);

        values.push_back(std::make_unique<instruction::MovInstruction>(std::move(ptrOperand), std::move(valueOperand), codegen::OperandSize::Long)); // TODO: Get size properly

        return nullptr;
    }

    StoreInst::StoreInst(BasicBlock* parent, Value* ptr, Value* value)
        : Instruction(parent->getParent()->getModule())
        , mPtr(ptr)
        , mValue(value)
    {
    }
}