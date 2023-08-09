// Copyright 2023 solar-mist


#include "vipir/Type/Type.h"
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
}