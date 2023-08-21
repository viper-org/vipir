// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vipir/Type/PointerType.h"

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

        PointerType* ptrType = static_cast<PointerType*>(mParent->getParent()->getValue(mPtr)->getType());
        codegen::OperandSize size = codegen::OperandSize::None;
        switch (ptrType->getBaseType()->getSizeInBits())
        {
            case 8:
                size = codegen::OperandSize::Byte;
                break;
            case 16:
                size = codegen::OperandSize::Word;
                break;
            case 32:
                size = codegen::OperandSize::Long;
                break;
            case 64:
                size = codegen::OperandSize::Quad;
                break;
        }

        values.push_back(std::make_unique<instruction::MovInstruction>(std::move(ptrOperand), std::move(valueOperand), size));
    }

    StoreInst::StoreInst(BasicBlock* parent, ValueId id, Value* ptr, Value* value)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mPtr(ptr->getID())
        , mValue(value->getID())
    {
        PointerType* ptrType = static_cast<PointerType*>(mParent->getParent()->getValue(mPtr)->getType());
        assert(ptrType->getBaseType() == value->getType());
    }
}