// Copyright 2024 solar-mist

/*
 * Function types represent the type of
 * functions themselves, eg int(int, char**)
*/

#ifndef VIPIR_TYPE_FUNCTION_TYPE_H
#define VIPIR_TYPE_FUNCTION_TYPE_H 1

#include "vipir/Type/Type.h"

#include <vector>

namespace vipir
{
    class FunctionType : public Type
    {
    public:
        FunctionType(Type* returnType, std::vector<Type*> argumentTypes);

        bool isFunctionType() const override;

        Type* getReturnType() const;
        const std::vector<Type*>& getArgumentTypes() const;

        static FunctionType* Create(Type* returnType, std::vector<Type*> argumentTypes);

    private:
        Type* mReturnType;
        std::vector<Type*> mArgumentTypes;
    };
}

#endif // VIPIR_TYPE_FUNCTION_TYPE_H