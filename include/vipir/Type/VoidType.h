// Copyright 2023 solar-mist

/*
 * Void is a type that cannot hold a value
 * and should only be used for functions
*/

#ifndef VIPIR_TYPE_VOID_TYPE_H
#define VIPIR_TYPE_VOID_TYPE_H 1

#include "vipir/Type/Type.h"

namespace vipir
{
    class VoidType : public Type
    {
    public:
        VoidType();

        std::size_t getSize() const;
        std::string_view getName() const;
    
    protected:
        std::size_t mSize;
        std::string mName;
    };
}

#endif // VIPIR_TYPE_VOID_TYPE_H