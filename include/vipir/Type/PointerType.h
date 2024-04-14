// Copyright 2024 solar-mist

/*
 * Pointer types are the type of a value
 * that references another value
*/

#ifndef VIPIR_TYPE_POINTER_TYPE_H
#define VIPIR_TYPE_POINTER_TYPE_H 1

#include "vipir/Type/Type.h"

namespace vipir
{
    class PointerType : public Type
    {
    public:
        PointerType(Type* baseType);

        bool isPointerType() const override;

        Type* getBaseType() const;

    private:
        Type* mBaseType;
    };
}

#endif // VIPIR_TYPE_POINTER_TYPE_H