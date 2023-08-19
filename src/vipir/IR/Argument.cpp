// Copyright 2023 solar-mist


#include "vipir/IR/Argument.h"

#include "vasm/instruction/operand/Register.h"

#include <format>
#include <iostream>

namespace vipir
{
    void Argument::print(std::ostream&) const
    {
    }

    std::string Argument::ident() const
    {
        return std::format("{} %{}", mType->getName(), mName);
    }

    bool Argument::requiresRegister() const
    {
        return true;
    }

    std::vector<ValueId> Argument::getOperands()
    {
        return {};
    }

    void Argument::emit(std::vector<instruction::ValuePtr>& values)
    {
        mEmittedValue = instruction::Register::Get(mRegister);
    }

    Argument::Argument(Module& module, ValueId id, Type* type, std::string name)
        : Value(module, id)
        , mName(name)
    {
        mType = type;
    }
}