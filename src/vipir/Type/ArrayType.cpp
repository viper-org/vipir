// Copyright 2024 solar-mist

#include "vipir/Type/ArrayType.h"

#include <format>

namespace vipir
{
    ArrayType::ArrayType(Type* baseType, int count)
        : Type(baseType->getSizeInBits() * count, std::format("[ {} x {} ]", count, baseType->getName()))
        , mBaseType(baseType)
        , mCount(count)
    {
        mAlignment = baseType->getAlignment();
    }

    bool ArrayType::isArrayType() const
    {
        return true;
    }

    Type* ArrayType::getBaseType() const
    {
        return mBaseType;
    }

    int ArrayType::getCount() const
    {
        return mCount;
    }
}