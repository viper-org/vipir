// Copyright 2024 solar-mist

/*
 * Struct types are composite types,
 * made up of other types
*/

#ifndef VIPIR_TYPE_STRUCT_TYPE_H
#define VIPIR_TYPE_STRUCT_TYPE_H 1

#include "vipir/Type/Type.h"

namespace vipir
{
    class StructType : public Type
    {
    public:
        StructType(std::vector<Type*> fields);

        bool isStructType() const override;

        const std::vector<Type*>& getFields() const;
        Type* getField(unsigned int num) const;

    private:
        std::vector<Type*> mFields;
    };
}

#endif // VIPIR_TYPE_STRUCT_TYPE_H