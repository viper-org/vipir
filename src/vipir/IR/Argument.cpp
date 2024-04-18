// Copyright 2024 solar-mist


#include "vipir/IR/Argument.h"

#include "vasm/instruction/operand/Register.h"

namespace vipir
{
    Argument::Argument(Module& module, Type* type, std::string&& name)
        : Value(module)
        , mName(std::move(name))
    {
        mType = type;
    }

    void Argument::print(std::ostream&)
    {
    }

    std::string Argument::ident() const
    {
        return "%" + mName;
    }

    void Argument::emit(MC::Builder&)
    {
        mEmittedValue = std::make_unique<instruction::Register>(mRegisterID, mType->getOperandSize());
    }
}