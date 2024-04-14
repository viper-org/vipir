// Copyright 2024 solar-mist

#include "vipir/Type/Type.h"
#include "vipir/Type/IntegerType.h"
#include "vipir/Type/VoidType.h"

#include <cassert>
#include <memory>
#include <vector>

namespace vipir
{
    std::vector<std::unique_ptr<vipir::Type>> types;

    Type* Type::GetIntegerType(int bits)
    {
        assert(bits % 8 == 0);
        for (const auto& type : types)
        {
            if (IntegerType* integerType = dynamic_cast<IntegerType*>(type.get()))
            {
                if (integerType->getSizeInBits() == bits)
                {
                    return integerType;
                }
            }
        }

        types.push_back(std::make_unique<IntegerType>(bits));
        return types.back().get();
    }

    Type* Type::GetVoidType()
    {
        for (const auto& type : types)
        {
            if (VoidType* voidType = dynamic_cast<VoidType*>(type.get()))
            {
                return voidType;
            }
        }

        types.push_back(std::make_unique<VoidType>());
        return types.back().get();
    }
}