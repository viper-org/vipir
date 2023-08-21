// Copyright 2023 solar-mist


#include "vipir/Type/PointerType.h"

namespace vipir
{
    using namespace std::literals;

    PointerType::PointerType(Type* baseType)
        : Type(64, baseType->getName().data() + "*"s)
        , mBase(baseType)
    {
    }

    Type* PointerType::getBaseType() const
    {
        return mBase;
    }

    PointerType* PointerType::Get(Type* baseType)
    {
        return static_cast<PointerType*>(Type::GetPointerType(baseType));
    }
}