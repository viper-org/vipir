// Copyright 2023 solar-mist


#include "vipir/Type/FunctionType.h"

namespace vipir
{
    using namespace std::literals;

    FunctionType::FunctionType(Type* returnType, const std::vector<Type*>& arguments)
        : Type(0, returnType->getName().data() + "()"s)
        , mReturnType(returnType)
        , mArguments(arguments)
    {
    }

    Type* FunctionType::getReturnType() const
    {
        return mReturnType;
    }

    const std::vector<Type*>& FunctionType::getArguments() const
    {
        return mArguments;
    }

    FunctionType* FunctionType::Get(Type* returnType, const std::vector<Type*>& arguments)
    {
        return static_cast<FunctionType*>(Type::GetFunctionType(returnType, arguments));
    }
}