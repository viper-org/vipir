// Copyright 2025 solar-mist

#ifndef VIPIR_DI_DI_TYPE_H
#define VIPIR_DI_DI_TYPE_H 1

#include "vipir/Type/Type.h"

#include <string>
#include <cstdint>

namespace vipir
{
    class DIType
    {
    friend class DIBuilder;
    public:
        virtual ~DIType() = default;
        
    protected:
        DIType() = default;

        int mOffset{ -1 };
    };

    class DIBasicType : public DIType
    {
    friend class DIBuilder;

        DIBasicType(std::string name, Type* type, uint8_t encoding);

        std::string mName;
        Type* mType;
        uint8_t mEncoding;
    };

    class DIPointerType : public DIType
    {
    friend class DIBuilder;

        DIPointerType(DIType* baseType /*, uint8_t byteSize*/);

        DIType* mBaseType;
    };
}

#endif // VIPIR_DI_DI_TYPE_H