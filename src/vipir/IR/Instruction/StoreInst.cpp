// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/GEPInst.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/GlobalVar.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/Relative.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"
#include <format>

namespace vipir
{
    void StoreInst::print(std::ostream& stream)
    {
        stream << std::format("store {} -> {}", mValue->ident(), mPtr->ident());
    }

    std::vector<Value*> StoreInst::getOperands()
    {
        return {mPtr, mValue};
    }

    std::string StoreInst::ident() const
    {
        return "%undef";
    }

    void StoreInst::emit(MC::Builder& builder)
    {
        instruction::OperandPtr ptr   = mPtr->getEmittedValue();
        instruction::OperandPtr value = mValue->getEmittedValue();

        if (auto labelOperand = dynamic_cast<instruction::LabelOperand*>(ptr.get()))
        {
            (void)ptr.release();
            instruction::LabelOperandPtr labelPtr = instruction::LabelOperandPtr(labelOperand);
            instruction::RelativePtr rel = std::make_unique<instruction::Relative>(std::move(labelPtr), std::nullopt);
            
            if (dynamic_cast<instruction::Immediate*>(value.get()))
            {
                // mov [rel], imm64 does not exist so we need to move it into a register first
                if (mValue->getType()->getOperandSize() == codegen::OperandSize::Quad)
                {
                    instruction::OperandPtr operand = mVReg->operand(codegen::OperandSize::Quad);
                    builder.addValue(std::make_unique<instruction::MovInstruction>(operand->clone(), std::move(value)));
                    value = std::move(operand);
                }
            }
            builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(rel), std::move(value), mValue->getType()->getOperandSize()));
        }

        else if (dynamic_cast<GEPInst*>(mValue))
        {
            builder.addValue(std::make_unique<instruction::LeaInstruction>(std::move(ptr), std::move(value), mValue->getType()->getOperandSize()));
        }
        else
        {
            if (dynamic_cast<AllocaInst*>(mPtr))
            {
                builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(ptr), std::move(value), mValue->getType()->getOperandSize()));
            }
            else
            {
                if (auto reg = dynamic_cast<instruction::Register*>(ptr.get()))
                {
                    (void)ptr.release();
                    instruction::RegisterPtr ptrReg = instruction::RegisterPtr(reg);
                    ptr = std::make_unique<instruction::Memory>(std::move(ptrReg), std::nullopt, nullptr, std::nullopt);
                }

                builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(ptr), std::move(value), mValue->getType()->getOperandSize()));
            }
        }
    }

    StoreInst::StoreInst(BasicBlock* parent, Value* ptr, Value* value)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mValue(value)
    {
        mRequiresVReg = false;
        if (dynamic_cast<GlobalVar*>(mPtr) && mValue->isConstant() && mValue->getType()->getOperandSize() == codegen::OperandSize::Quad)
        {
            mRequiresVReg = true;
        }
    }
}