// Copyright 2023 solar-mist


#include "vipir/Type/Type.h"
#include "vipir/Type/IntegerType.h"
#include "vipir/Type/VoidType.h"
#include "vipir/Type/FunctionType.h"
#include "vipir/Type/PointerType.h"

#include <memory>
#include <vector>

namespace vipir
{
    std::vector<std::unique_ptr<vipir::Type>> types;

    Type* Type::GetVoidType()
    {
        for (const auto& type : types)
        {
            if (dynamic_cast<VoidType*>(type.get()))
            {
                return type.get();
            }
        }

        types.push_back(std::make_unique<VoidType>());
        return types.back().get();
    }

    Type* Type::GetIntegerType(int bits)
    {
        for (const auto& type : types)
        {
            if (IntegerType* integerType = dynamic_cast<IntegerType*>(type.get()))
            {
                if (integerType->getSizeInBits() == bits)
                {
                    return integerType;
                }
            }
        }

        types.push_back(std::make_unique<IntegerType>(bits));
        return types.back().get();
    }

    Type* Type::GetFunctionType(Type* returnType, const std::vector<Type*>& arguments)
    {
        for (const auto& type : types)
        {
            if (FunctionType* functionType = dynamic_cast<FunctionType*>(type.get()))
            {
                if (functionType->getReturnType() == returnType && functionType->getArguments() == arguments)
                {
                    return functionType;
                }
            }
        }

        types.push_back(std::make_unique<FunctionType>(returnType, arguments));
        return types.back().get();
    }

    Type* Type::GetPointerType(Type* baseType)
    {
        for (const auto& type : types)
        {
            if (PointerType* pointerType = dynamic_cast<PointerType*>(type.get()))
            {
                if (pointerType->getBaseType() == baseType)
                {
                    return pointerType;
                }
            }
        }

        types.push_back(std::make_unique<PointerType>(baseType));
        return types.back().get();
    }
}