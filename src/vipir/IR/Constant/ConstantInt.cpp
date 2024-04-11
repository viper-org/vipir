// Copyright 2024 solar-mist

#include "vipir/IR/Constant/ConstantInt.h"

#include "vipir/IR/BasicBlock.h"

#include "vasm/instruction/operand/Immediate.h"

#include <format>

namespace vipir
{
    void ConstantInt::print(std::ostream&)
    {
    }

    std::string ConstantInt::ident() const
    {
        return std::format("i32 {}", mValue);
    }


    void ConstantInt::emit(MC::Builder& builder)
    {
        mEmittedValue = std::make_unique<instruction::Immediate>(mValue);
    }

    ConstantInt::ConstantInt(BasicBlock* parent, intmax_t value)
        : Value(parent->getModule())
        , mValue(value)
    {
    }
}