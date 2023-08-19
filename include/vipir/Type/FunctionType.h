// Copyright 2023 solar-mist

/*
 * Function types are the type of a function
 * (NOT the return type)
*/

#ifndef VIPIR_TYPE_FUNCTION_TYPE_H
#define VIPIR_TYPE_FUNCTION_TYPE_H 1

#include "vipir/Type/Type.h"

namespace vipir
{
    // TODO: Add function arguments
    class FunctionType : public Type
    {
    public:
        FunctionType(Type* returnType);

        Type* getReturnType() const;

        static FunctionType* Get(Type* returnType);

    private:
        Type* mReturnType;
    };
}

#endif // VIPIR_TYPE_FUNCTION_TYPE_H