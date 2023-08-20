// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    void StoreInst::print(std::ostream& stream) const
    {
        stream << std::format("store {}, {}", mParent->getParent()->getValue(mPtr)->ident(), mParent->getParent()->getValue(mValue)->ident());
    }

    std::string StoreInst::ident() const
    {
        return "%undef";
    }

    bool StoreInst::requiresRegister() const
    {
        return false;
    }

    std::vector<ValueId> StoreInst::getOperands()
    {
        return {mPtr, mValue};
    }

    void StoreInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        instruction::OperandPtr ptrOperand   = mParent->getEmittedValue(mPtr);
        instruction::OperandPtr valueOperand = mParent->getEmittedValue(mValue);

        values.push_back(std::make_unique<instruction::MovInstruction>(std::move(ptrOperand), std::move(valueOperand), codegen::OperandSize::Long)); // TODO: Get size properly
    }

    StoreInst::StoreInst(BasicBlock* parent, ValueId id, Value* ptr, Value* value)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mPtr(ptr->getID())
        , mValue(value->getID())
    {
        assert(static_cast<AllocaInst*>(ptr)->getAllocatedType() == value->getType());
    }
}