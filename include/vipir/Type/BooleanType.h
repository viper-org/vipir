// Copyright 2024 solar-mist

/*
 * Boolean is a type that only stores a true
 * or false value
*/

#ifndef VIPIR_TYPE_BOOLEAN_TYPE_H
#define VIPIR_TYPE_BOOLEAN_TYPE_H 1

#include "vipir/Type/Type.h"

namespace vipir
{
    class BooleanType : public Type
    {
    public:
        BooleanType();
    };
}

#endif // VIPIR_TYPE_BOOLEAN_TYPE_H