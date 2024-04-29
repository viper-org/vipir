// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/LoadInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Register.h"

#include "vipir/Type/PointerType.h"

#include <cassert>
#include <format>

namespace vipir
{
    void LoadInst::print(std::ostream& stream)
    {
        stream << std::format("load {} %{}, {}", mType->getName(), mValueId, mPtr->ident());
    }

    Value* LoadInst::getPointer()
    {
        return mPtr;
    }

    std::string LoadInst::ident() const
    {
        return std::format("%{}", mValueId);
    }

    void LoadInst::emit(MC::Builder& builder)
    {
        codegen::OperandSize size = mType->getOperandSize();
        
        instruction::OperandPtr ptr = mPtr->getEmittedValue();
        instruction::OperandPtr reg = std::make_unique<instruction::Register>(mRegisterID, size);

        if (dynamic_cast<instruction::Memory*>(ptr.get()))
        {
            builder.addValue(std::make_unique<instruction::MovInstruction>(reg->clone(), std::move(ptr)));
        }
        else if (auto regOperand = dynamic_cast<instruction::Register*>(ptr.get()))
        {
            (void)ptr.release();
            instruction::RegisterPtr ptrReg = instruction::RegisterPtr(regOperand);
            instruction::OperandPtr memory = std::make_unique<instruction::Memory>(std::move(ptrReg), std::nullopt);
            builder.addValue(std::make_unique<instruction::MovInstruction>(reg->clone(), std::move(memory)));
        }

        mEmittedValue = std::move(reg);
    }

    LoadInst::LoadInst(BasicBlock* parent, Value* ptr)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mValueId(mModule.getNextValueId())
    {
        assert(mPtr->getType()->isPointerType());
        mType = static_cast<PointerType*>(mPtr->getType())->getBaseType();

        requiresRegister = true;
    }
}