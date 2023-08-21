// Copyright 2023 solar-mist

/*
 * Pointer types point to another type(such as an i32)
*/

#ifndef VIPIR_TYPE_POINTER_TYPE_H
#define VIPIR_TYPE_POINTER_TYPE_H 1

#include "vipir/Type/Type.h"

#include <vector>

namespace vipir
{
    // TODO: Add function arguments
    class PointerType : public Type
    {
    public:
        PointerType(Type* returnType);

        Type* getBaseType() const;

        static PointerType* Get(Type* baseType);

    private:
        Type* mBase;
    };
}

#endif // VIPIR_TYPE_POINTER_TYPE_H