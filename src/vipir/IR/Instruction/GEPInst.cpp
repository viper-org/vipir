// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/GEPInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Type/PointerType.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/operand/Memory.h"

#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"

#include <format>

namespace vipir
{
    void GEPInst::print(std::ostream& stream)
    {
        stream << std::format("gep %{}, {}, {}", mValueId, mPtr->ident(), mOffset->ident());
    }

    std::string GEPInst::ident() const
    {
        return std::format("%{}", mValueId);
    }

    std::vector<Value*> GEPInst::getOperands()
    {
        return std::vector<Value*>{mOffset};
    }

    void GEPInst::emit(MC::Builder& builder)
    {
        codegen::OperandSize size = mType->getOperandSize();
        instruction::OperandPtr reg = std::make_unique<instruction::Register>(mRegisterID, size);

        instruction::OperandPtr ptr = mPtr->getEmittedValue();
        instruction::OperandPtr offset = mOffset->getEmittedValue();

        instruction::Register* ptrReg = static_cast<instruction::Register*>(ptr.get());

        int scale = static_cast<PointerType*>(mType)->getBaseType()->getSizeInBits() / 8;

        if (auto immediate = dynamic_cast<instruction::Immediate*>(offset.get()))
        {
            int displacement = immediate->imm64() * scale;
            (void)ptr.release();

            instruction::OperandPtr memory = std::make_unique<instruction::Memory>(instruction::RegisterPtr(ptrReg), displacement, nullptr, std::nullopt);
            builder.addValue(std::make_unique<instruction::LeaInstruction>(reg->clone(), std::move(memory)));
            mEmittedValue = std::move(reg);
        }
        else if (auto regOffset = dynamic_cast<instruction::Register*>(offset.get()))
        {
            (void)ptr.release();
            instruction::Register* index = regOffset;
            (void)offset.release();

            instruction::OperandPtr memory = std::make_unique<instruction::Memory>(instruction::RegisterPtr(ptrReg), std::nullopt, instruction::RegisterPtr(regOffset), scale);
            builder.addValue(std::make_unique<instruction::LeaInstruction>(reg->clone(), std::move(memory)));
            mEmittedValue = std::move(reg);
        }
    }

    GEPInst::GEPInst(BasicBlock* parent, Value* ptr, Value* offset)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mOffset(offset)
        , mValueId(mModule.getNextValueId())
    {
        mType = mPtr->getType();

        assert(mType->isPointerType());

        requiresRegister = true;
    }
}