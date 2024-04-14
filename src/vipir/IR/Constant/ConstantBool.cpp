// Copyright 2024 solar-mist

#include "vipir/IR/Constant/ConstantBool.h"

#include "vipir/IR/BasicBlock.h"

#include "vasm/instruction/operand/Immediate.h"

#include <format>

namespace vipir
{
    void ConstantBool::print(std::ostream&)
    {
    }

    std::string ConstantBool::ident() const
    {
        return std::format("{} {}", mType->getName(), mValue);
    }


    void ConstantBool::emit(MC::Builder& builder)
    {
        mEmittedValue = std::make_unique<instruction::Immediate>(static_cast<int>(mValue));
    }

    ConstantBool::ConstantBool(BasicBlock* parent, bool value)
        : Value(parent->getModule())
        , mValue(value)
    {
        mType = Type::GetBooleanType();
    }
}