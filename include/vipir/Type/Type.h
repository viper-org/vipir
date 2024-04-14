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

        static Type* GetIntegerType(int bits);
        static Type* GetVoidType();

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