// Copyright 2024 solar-mist

/*
 * Integer types are types that store numbers with no
 * floating point, i.e. 6 or 42
*/

#ifndef VIPIR_TYPE_INTEGER_TYPE_H
#define VIPIR_TYPE_INTEGER_TYPE_H 1

#include "vipir/Type/Type.h"

namespace vipir
{
    class IntegerType : public Type
    {
    public:
        IntegerType(int bits);

        bool isIntegerType() const override;
    };
}

#endif // VIPIR_TYPE_INTEGER_TYPE_H