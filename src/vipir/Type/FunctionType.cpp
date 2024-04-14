// Copyright 2024 solar-mist

#include "vipir/Type/FunctionType.h"

#include <format>

namespace vipir
{
    FunctionType::FunctionType(Type* returnType)
        : Type(0, std::format("{}()", returnType->getName()))
        , mReturnType(returnType)
    {
    }

    bool FunctionType::isFunctionType() const
    {
        return true;
    }

    Type* FunctionType::getReturnType() const
    {
        return mReturnType;
    }

    FunctionType* FunctionType::Create(Type* returnType)
    {
        return static_cast<FunctionType*>(Type::GetFunctionType(returnType));
    }
}