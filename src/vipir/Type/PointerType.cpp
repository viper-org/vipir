// Copyright 2024 solar-mist

#include "vipir/Type/PointerType.h"

#include <format>

namespace vipir
{
    PointerType::PointerType(Type* baseType)
        : Type(64, std::format("{}*", baseType->getName()))
        , mBaseType(baseType)
    {
        mOperandSize = codegen::OperandSize::Quad;
    }

    bool PointerType::isPointerType() const
    {
        return true;
    }

    Type* PointerType::getBaseType() const
    {
        return mBaseType;
    }
}