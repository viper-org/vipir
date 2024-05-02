// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/SExtInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    void SExtInst::print(std::ostream& stream)
    {
        stream << std::format("sext {} -> {} %{}", mValue->ident(), mType->getName(), mValueId);
    }

    std::string SExtInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), mValueId);
    }

    std::vector<Value*> SExtInst::getOperands()
    {
        return {mValue};
    }

    
    void SExtInst::emit(MC::Builder& builder)
    {
        instruction::OperandPtr value = mValue->getEmittedValue();
        instruction::RegisterPtr reg = std::make_unique<instruction::Register>(mRegisterID, mType->getOperandSize());

        builder.addValue(std::make_unique<instruction::MovSXInstruction>(reg->clone(), std::move(value)));

        mEmittedValue = std::move(reg);
    }


    SExtInst::SExtInst(BasicBlock* parent, Value* ptr, Type* destType)
        : Instruction(parent->getModule(), parent)
        , mValue(ptr)
        , mValueId(mModule.getNextValueId())
    {
        assert(destType->isIntegerType());
        assert(mValue->getType()->isIntegerType());
        assert(mValue->getType()->getSizeInBits() < destType->getSizeInBits());

        mType = destType;

        requiresRegister = true;
    }
}