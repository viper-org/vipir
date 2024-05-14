// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/StoreParamInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"

#include <format>

namespace vipir
{
    void StoreParamInst::print(std::ostream& stream)
    {
        stream << std::format("store {} -> PARAM{}", mValue->ident(), mParamIndex);
    }

    std::vector<Value*> StoreParamInst::getOperands()
    {
        return {mValue};
    }

    std::vector<int> StoreParamInst::getRegisterSmashes()
    {
        return {mModule.abi()->getParameterRegister(mParamIndex)};
    }

    std::string StoreParamInst::ident() const
    {
        return "%undef";
    }


    void StoreParamInst::emit(lir::Builder& builder)
    {
        mValue->lateEmit(builder);

        lir::OperandPtr ptr = std::make_unique<lir::PhysicalReg>(mModule.abi()->getParameterRegister(mParamIndex), mValue->getType()->getOperandSize());
        lir::OperandPtr value = mValue->getEmittedValue();

        builder.addValue(std::make_unique<lir::Move>(std::move(ptr), std::move(value)));
    }

    StoreParamInst::StoreParamInst(BasicBlock* parent, int paramIndex, Value* value)
        : Instruction(parent->getModule(), parent)
        , mParamIndex(paramIndex)
        , mValue(value)
    {
        mRequiresVReg = false;
    }
}