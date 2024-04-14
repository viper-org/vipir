// Copyright 2024 solar-mist

/*
 * Type is the base class for all types in vipir
 * such as an integral or struct type
*/

#ifndef VIPIR_TYPE_TYPE_H
#define VIPIR_TYPE_TYPE_H 1

#include "vasm/codegen/Opcodes.h"

#include <string>

namespace vipir
{
    class Type
    {
    public:
        virtual ~Type() { }

        std::size_t getSizeInBits() { return mSizeInBits; }
        std::string_view getName() { return mName; }
        codegen::OperandSize getOperandSize() { return mOperandSize; }

        virtual bool isIntegerType()  const { return false; }
        virtual bool isVoidType()     const { return false; }
        virtual bool isFunctionType() const { return false; }
        virtual bool isPointerType()  const { return false; }

        static Type* GetIntegerType(int bits);
        static Type* GetVoidType();
        static Type* GetBooleanType();
        static Type* GetFunctionType(Type* returnType); // TODO: Add argument types
        static Type* GetPointerType(Type* baseType);

    protected:
        Type(std::size_t sizeInBits, std::string_view name)
            : mSizeInBits(sizeInBits)
            , mName(name)
        {
        }

        std::size_t mSizeInBits;
        std::string mName;

        codegen::OperandSize mOperandSize;
    };
}

#endif // VIPIR_TYPE_TYPE