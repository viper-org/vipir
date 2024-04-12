// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vasm/instruction/operand/Register.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    static int vregCount = 1;
    void LoadInst::print(std::ostream& stream)
    {
        stream << std::format("load i32 %vreg{}, {}", mVregNum, mPtr->ident());
    }

    std::string LoadInst::ident() const
    {
        return std::format("%vreg{}", mVregNum);
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
    }

    LoadInst::LoadInst(BasicBlock* parent, Value* ptr)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mVregNum(vregCount++)
    {
        requiresRegister = true;
    }
}