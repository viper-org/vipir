// Copyright 2024 solar-mist


#include "vipir/IR/Constant/ConstantNullPtr.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Immediate.h"

#include <format>

namespace vipir
{
    ConstantNullPtr* ConstantNullPtr::Get(Module& module, Type* type)
    {
        ConstantNullPtr* constant = new ConstantNullPtr(module, type);

        module.insertConstant(constant);

        return constant;
    }

    void ConstantNullPtr::print(std::ostream& stream)
    {
    }

    std::string ConstantNullPtr::ident() const
    {
        return std::format("{} null", mType->getName());
    }

    bool ConstantNullPtr::isConstant() const
    {
        return true;
    }

    void ConstantNullPtr::emit(MC::Builder& builder)
    {
        mEmittedValue = std::make_unique<instruction::Immediate>(0);
    }

    ConstantNullPtr::ConstantNullPtr(Module& module, Type* type)
        : Value(module)
    {
        assert(type->isPointerType());
        mType = type;
    }
}