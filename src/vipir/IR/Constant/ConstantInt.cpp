// Copyright 2024 solar-mist


#include "vipir/IR/Constant/ConstantInt.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Immediate.h"

#include <format>

namespace vipir
{
    ConstantInt* ConstantInt::Get(Module& module, intmax_t value, Type* type)
    {
        ConstantInt* constant = new ConstantInt(module, value, type);
        module.insertConstant(constant);
        return constant;
    }

    void ConstantInt::print(std::ostream&)
    {
    }

    std::string ConstantInt::ident() const
    {
        return std::format("{} {}", mType->getName(), mValue);
    }

    bool ConstantInt::isConstant() const
    {
        return true;
    }

    intmax_t ConstantInt::getValue() const
    {
        return mValue;
    }


    void ConstantInt::emit(MC::Builder& builder)
    {
        mEmittedValue = std::make_unique<instruction::Immediate>(mValue);
    }

    ConstantInt::ConstantInt(BasicBlock* parent, intmax_t value, Type* type)
        : Value(parent->getModule())
        , mValue(value)
    {
        mType = type;
    }
    ConstantInt::ConstantInt(Module& module, intmax_t value, Type* type)
        : Value(module)
        , mValue(value)
    {
        mType = type;
    }
}