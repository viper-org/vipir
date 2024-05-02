// Copyright 2024 solar-mist

/*
 * Type is the base class for all types in vipir
 * such as an integral or struct type
*/

#ifndef VIPIR_TYPE_TYPE_H
#define VIPIR_TYPE_TYPE_H 1

#include "vasm/codegen/Opcodes.h"

#include <string>
#include <vector>

namespace vipir
{
    class Type
    {
    public:
        virtual ~Type() { }

        std::size_t getSizeInBits() const { return mSizeInBits; }
        std::size_t getAlignment() const { return mAlignment; }
        std::string_view getName() { return mName; } const
        codegen::OperandSize getOperandSize() const { return mOperandSize; }

        virtual bool isIntegerType()  const { return false; }
        virtual bool isVoidType()     const { return false; }
        virtual bool isFunctionType() const { return false; }
        virtual bool isPointerType()  const { return false; }
        virtual bool isStructType()   const { return false; }
        virtual bool isArrayType()    const { return false; }

        static Type* GetIntegerType(int bits);
        static Type* GetVoidType();
        static Type* GetBooleanType();
        static Type* GetFunctionType(Type* returnType, std::vector<Type*> argumentTypes);
        static Type* GetPointerType(Type* baseType);
        static Type* GetStructType(std::vector<Type*> fieldTypes);
        static Type* GetArrayType(Type* baseType, int count);

    protected:
        Type(std::size_t sizeInBits, std::string_view name)
            : mSizeInBits(sizeInBits)
            , mName(name)
            , mAlignment(mSizeInBits)
            , mOperandSize(codegen::OperandSize::None)
        {
        }

        std::size_t mSizeInBits;
        std::size_t mAlignment;
        std::string mName;

        codegen::OperandSize mOperandSize;
    };
}

#endif // VIPIR_TYPE_TYPE