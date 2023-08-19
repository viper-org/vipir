// Copyright 2023 solar-mist

/*
 * A type is the base class for all types in vipir
 * such as an integral or struct type
*/

#ifndef VIPIR_TYPE_TYPE_H
#define VIPIR_TYPE_TYPE_H 1

#include <string>
#include <vector>

namespace vipir
{
    class Type
    {
    public:
        virtual ~Type() { }

        std::size_t getSizeInBits() const { return mSize; }
        std::string_view getName() const { return mName; }

        static Type* GetVoidType();
        static Type* GetIntegerType(int bits);
        static Type* GetFunctionType(Type* returnType, const std::vector<Type*>& arguments);
    
    protected:
        Type(std::size_t size, std::string name)
            : mSize(size)
            , mName(name)
        {
        }

        std::size_t mSize;
        std::string mName;
    };
}

#endif // VIPIR_TYPE_TYPE_H