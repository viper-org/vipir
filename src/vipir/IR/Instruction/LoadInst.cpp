// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Type/PointerType.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vipir/IR/Instruction/Instruction.h"
#include "vasm/instruction/operand/Memory.h"

#include <format>
#include <iostream>

namespace vipir
{
    void LoadInst::print(std::ostream& stream) const
    {
        stream << std::format("%{} = load {}", mName, mParent->getParent()->getValue(mPtr)->ident());
    }

    std::string LoadInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), mName);
    }

    bool LoadInst::requiresRegister() const
    {
        return true;
    }

    std::vector<ValueId> LoadInst::getOperands()
    {
        return {mPtr};
    }
    
    Value* LoadInst::getPointer() const
    {
        return mParent->getParent()->getValue(mPtr);
    }

    void LoadInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        instruction::OperandPtr ptrOperand = mParent->getEmittedValue(mPtr);

        if (dynamic_cast<instruction::Memory*>(ptrOperand.get()))
        {
            values.emplace_back(std::make_unique<instruction::MovInstruction>(instruction::Register::Get(mRegister), std::move(ptrOperand), codegen::OperandSize::None));
        }
        else
        {
            instruction::Register* reg = static_cast<instruction::Register*>(ptrOperand.get());
            instruction::RegisterPtr regPtr = std::make_unique<instruction::Register>(reg->getID(), reg->getSize());
            instruction::OperandPtr memory = std::make_unique<instruction::Memory>(std::move(regPtr), std::nullopt);

            values.emplace_back(std::make_unique<instruction::MovInstruction>(instruction::Register::Get(mRegister), std::move(memory), codegen::OperandSize::None));
        }

        mEmittedValue = instruction::Register::Get(mRegister);
    }

    LoadInst::LoadInst(BasicBlock* parent, ValueId id, Value* ptr, std::string name)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mPtr(ptr->getID())
        , mName(name)
    {
        mType = static_cast<PointerType*>(mParent->getParent()->getValue(mPtr)->getType())->getBaseType();
    }
}