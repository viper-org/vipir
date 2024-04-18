// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/CallInst.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Register.h"

#include "vasm/instruction/singleOperandInstruction/CallInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    void CallInst::print(std::ostream& stream)
    {
        stream << std::format("call {}(), %{}", mFunction->ident(), mValueId);
    }

    std::string CallInst::ident() const
    {
        return std::format("%{}", mValueId);
    }

    void CallInst::emit(MC::Builder& builder)
    {
        instruction::OperandPtr function = mFunction->getEmittedValue();
        codegen::OperandSize size = mType->getOperandSize();

        builder.addValue(std::make_unique<instruction::CallInstruction>(std::move(function)));
        
        instruction::OperandPtr reg = std::make_unique<instruction::Register>(mRegisterID, size);
        if (mRegisterID != 0) // eax
        {
            instruction::OperandPtr eax = std::make_unique<instruction::Register>(0, size);
            builder.addValue(std::make_unique<instruction::MovInstruction>(reg->clone(), std::move(eax)));
        }
        mEmittedValue = std::move(reg);
    }

    CallInst::CallInst(BasicBlock* parent, Function* function)
        : Instruction(parent->getModule(), parent)
        , mFunction(function)
        , mValueId(mModule.getNextValueId())
    {
        mType = mFunction->getFunctionType()->getReturnType();

        requiresRegister = true;
        mRegisterSmashes.push_back(0); // eax
    }
}