// Copyright 2024 solar-mist

/*
 * Void is a type that cannot hold a value
 * and should only be used for function
 * return values
*/

#ifndef VIPIR_TYPE_VOID_TYPE_H
#define VIPIR_TYPE_VOID_TYPE_H 1

#include "vipir/Type/Type.h"

namespace vipir
{
    class VoidType : public Type
    {
    public:
        VoidType();

        bool isVoidType() const override;
    };
}

#endif // VIPIR_TYPE_VOID_TYPE_H