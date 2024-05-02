// Copyright 2024 solar-mist

/*
 * Array types are the type of a value
 * that contains a constant number of
 * values of the same time
*/

#ifndef VIPIR_TYPE_ARRAY_TYPE_H
#define VIPIR_TYPE_ARRAY_TYPE_H 1

#include "vipir/Type/Type.h"

namespace vipir
{
    class ArrayType : public Type
    {
    public:
        ArrayType(Type* baseType, int count);

        bool isArrayType() const override;

        Type* getBaseType() const;
        int getCount() const;

    private:
        Type* mBaseType;
        int mCount;
    };
}

#endif // VIPIR_TYPE_ARRAY_TYPE_H