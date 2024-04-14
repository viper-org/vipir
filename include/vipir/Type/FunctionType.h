// Copyright 2024 solar-mist

/*
 * Function types represent the type of
 * functions themselves, eg int(int, char**)
*/

#ifndef VIPIR_TYPE_FUNCTION_TYPE_H
#define VIPIR_TYPE_FUNCTION_TYPE_H 1

#include "vipir/Type/Type.h"

namespace vipir
{
    // TODO: Add arguments
    class FunctionType : public Type
    {
    public:
        FunctionType(Type* returnType);

        Type* getReturnType() const;

        static FunctionType* Create(Type* returnType); // TODO: Add arguments

    private:
        Type* mReturnType;
    };
}

#endif // VIPIR_TYPE_FUNCTION_TYPE_H