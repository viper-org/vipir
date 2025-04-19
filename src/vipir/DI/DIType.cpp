// Copyright 2025 solar-mist

#include "vipir/DI/DIType.h"

namespace vipir
{
    DIBasicType::DIBasicType(std::string name, Type* type, uint8_t encoding)
        : mName(std::move(name))
        , mType(type)
        , mEncoding(encoding)
    {
    }


    DIPointerType::DIPointerType(DIType* baseType)
        : mBaseType(baseType)
    {
    }
}