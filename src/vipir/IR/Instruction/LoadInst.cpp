// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/LoadInst.h"

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
        instruction::OperandPtr& ptr = mPtr->getEmittedValue();
        instruction::OperandPtr  reg = std::make_unique<instruction::Register>(mRegisterID, codegen::OperandSize::Long);

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