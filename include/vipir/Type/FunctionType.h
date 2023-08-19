// Copyright 2023 solar-mist

/*
 * Function types are the type of a function
 * (NOT the return type)
*/

#ifndef VIPIR_TYPE_FUNCTION_TYPE_H
#define VIPIR_TYPE_FUNCTION_TYPE_H 1

#include "vipir/Type/Type.h"

#include <vector>

namespace vipir
{
    // TODO: Add function arguments
    class FunctionType : public Type
    {
    public:
        FunctionType(Type* returnType, const std::vector<Type*>& arguments);

        Type* getReturnType() const;
        const std::vector<Type*>& getArguments() const;

        static FunctionType* Get(Type* returnType, const std::vector<Type*>& arguments);

    private:
        Type* mReturnType;
        std::vector<Type*> mArguments;
    };
}

#endif // VIPIR_TYPE_FUNCTION_TYPE_H