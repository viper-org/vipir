// Copyright 2023 solar-mist


#include "vipir/Type/Type.h"
#include "vipir/Type/IntegerType.h"
#include "vipir/Type/VoidType.h"

#include <vector>

namespace vipir
{
    std::vector<vipir::Type> types;

    Type* Type::GetVoidType()
    {
        for (Type& type : types)
        {
            if (dynamic_cast<VoidType*>(&type))
            {
                return &type;
            }
        }

        types.push_back(VoidType());
        return &types.back();
    }

    Type* Type::GetIntegerType(int bits)
    {
        for (Type& type : types)
        {
            if (IntegerType* integerType = dynamic_cast<IntegerType*>(&type))
            {
                if (integerType->getSizeInBits() == bits)
                {
                    return integerType;
                }
            }
        }

        types.push_back(IntegerType(bits));
        return &types.back();
    }
}