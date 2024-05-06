// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/LoadAddress.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Relative.h"
#include "vasm/instruction/operand/Label.h"

#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    void AddrInst::print(std::ostream& stream)
    {
        stream << std::format("addr {} %{}, {}", mType->getName(), getName(mValueId), mPtr->ident());
    }

    Value* AddrInst::getPointer()
    {
        return mPtr;
    }

    std::string AddrInst::ident() const
    {
        return std::format("%{}", getName(mValueId));
    }

    void AddrInst::emit(MC::Builder& builder)
    {
        codegen::OperandSize size = mType->getOperandSize();
        
        instruction::OperandPtr ptr = mPtr->getEmittedValue();
        instruction::OperandPtr operand = mVReg->operand(size);
        if (auto labelOperand = dynamic_cast<instruction::LabelOperand*>(ptr.get()))
        {
            (void)ptr.release();
            instruction::LabelOperandPtr labelPtr = instruction::LabelOperandPtr(labelOperand);
            instruction::RelativePtr rel = std::make_unique<instruction::Relative>(std::move(labelPtr), std::nullopt);
            builder.addValue(std::make_unique<instruction::LeaInstruction>(operand->clone(), std::move(rel)));
        }
        else
        {
            builder.addValue(std::make_unique<instruction::LeaInstruction>(operand->clone(), std::move(ptr)));
        }

        mEmittedValue = std::move(operand);
    }

    void AddrInst::emit2(lir::Builder& builder)
    {
        mPtr->lateEmit(builder);

        lir::OperandPtr ptr = mPtr->getEmittedValue2();
        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
        builder.addValue(std::make_unique<lir::LoadAddress>(vreg->clone(), std::move(ptr)));

        mEmittedValue2 = std::move(vreg);
    }

    AddrInst::AddrInst(BasicBlock* parent, Value* ptr)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mValueId(mModule.getNextValueId())
    {
        mType = mPtr->getType();

        if (auto alloca = dynamic_cast<AllocaInst*>(mPtr))
        {
            alloca->forceMemory();
        }
    }
}