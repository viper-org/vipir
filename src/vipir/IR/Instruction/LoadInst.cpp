// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Register.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    void LoadInst::print(std::ostream& stream)
    {
        stream << std::format("load i32 %{}, {}", mValueId, mPtr->ident());
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
        codegen::OperandSize size = codegen::OperandSize::None; // TODO: Get this properly from pointer type
        if (AllocaInst* alloca = dynamic_cast<AllocaInst*>(mPtr))
        {
            size = alloca->getAllocatedType()->getOperandSize();
        }
        
        instruction::OperandPtr& ptr = mPtr->getEmittedValue();
        instruction::OperandPtr  reg = std::make_unique<instruction::Register>(mRegisterID, size);

        builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(reg), std::move(ptr)));

        mEmittedValue = std::move(reg);
    }

    LoadInst::LoadInst(BasicBlock* parent, Value* ptr)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mValueId(mModule.getNextValueId())
    {
        requiresRegister = true;
        if (AllocaInst* alloca = dynamic_cast<AllocaInst*>(mPtr)) // TODO: Get this properly from pointer type
        {
            mType = alloca->getAllocatedType();
        }
    }
}