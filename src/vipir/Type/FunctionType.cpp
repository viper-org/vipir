// Copyright 2023 solar-mist


#include "vipir/Type/FunctionType.h"

namespace vipir
{
    using namespace std::literals;

    FunctionType::FunctionType(Type* returnType)
        : Type(0, returnType->getName().data() + "()"s)
        , mReturnType(returnType)
    {
    }

    Type* FunctionType::getReturnType() const
    {
        return mReturnType;
    }

    FunctionType* FunctionType::Get(Type* returnType)
    {
        return static_cast<FunctionType*>(Type::GetFunctionType(returnType));
    }
}