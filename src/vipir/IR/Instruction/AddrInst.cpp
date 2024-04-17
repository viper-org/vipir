// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Register.h"

#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    void AddrInst::print(std::ostream& stream)
    {
        stream << std::format("addr {} %{}, {}", mType->getName(), mValueId, mPtr->ident());
    }

    Value* AddrInst::getPointer()
    {
        return mPtr;
    }

    std::string AddrInst::ident() const
    {
        return std::format("%{}", mValueId);
    }

    void AddrInst::emit(MC::Builder& builder)
    {
        codegen::OperandSize size = mType->getOperandSize();
        
        instruction::OperandPtr ptr = mPtr->getEmittedValue();
        instruction::OperandPtr reg = std::make_unique<instruction::Register>(mRegisterID, size);

        builder.addValue(std::make_unique<instruction::LeaInstruction>(reg->clone(), std::move(ptr)));

        mEmittedValue = std::move(reg);
    }

    AddrInst::AddrInst(BasicBlock* parent, Value* ptr)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mValueId(mModule.getNextValueId())
    {
        assert(dynamic_cast<AllocaInst*>(mPtr) != nullptr);
        mType = mPtr->getType();

        requiresRegister = true;
    }
}