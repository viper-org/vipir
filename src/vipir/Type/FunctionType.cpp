// Copyright 2024 solar-mist

#include "vipir/Type/FunctionType.h"

#include <format>

namespace vipir
{
    FunctionType::FunctionType(Type* returnType, std::vector<Type*> argumentTypes)
        : Type(0, std::format("{}(", returnType->getName()))
        , mReturnType(returnType)
        , mArgumentTypes(std::move(argumentTypes))
    {
        for (auto i = 0; i < mArgumentTypes.size() - 1; ++i)
        {
            mName += std::string(mArgumentTypes[i]->getName()) + ", ";
        }
        mName += std::string(mArgumentTypes.back()->getName()) + ')';
    }

    bool FunctionType::isFunctionType() const
    {
        return true;
    }

    Type* FunctionType::getReturnType() const
    {
        return mReturnType;
    }

    const std::vector<Type*>& FunctionType::getArgumentTypes() const
    {
        return mArgumentTypes;
    }

    FunctionType* FunctionType::Create(Type* returnType, std::vector<Type*> argumentTypes)
    {
        return static_cast<FunctionType*>(Type::GetFunctionType(returnType, argumentTypes));
    }
}