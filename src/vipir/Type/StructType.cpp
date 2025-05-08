// Copyright 2024 solar-mist

#include "vipir/Type/StructType.h"

#include <format>

namespace vipir
{
    constexpr std::size_t AlignUp(std::size_t value, std::size_t align)
    {
        return (value + align - 1) & ~(align - 1);
    }

    StructType::StructType(std::vector<Type*> fields)
        : Type(0, "struct { ")
        , mFields(std::move(fields))
    {
        mOperandSize = codegen::OperandSize::Quad;
        std::size_t maxAlignment = 0;
        for (auto field : mFields)
        {
            if (field->getAlignment() > maxAlignment)
            {
                maxAlignment = field->getAlignment();
            }
        }
        mAlignment = maxAlignment;

        for (auto field : mFields)
        {
            mSizeInBits += AlignUp(field->getSizeInBits(), maxAlignment);
            mName += std::string(field->getName()) + " ";
        }
        mName += "}";
    }

    bool StructType::isStructType() const
    {
        return true;
    }

    const std::vector<Type*>& StructType::getFields() const
    {
        return mFields;
    }

    Type* StructType::getField(unsigned int num) const
    {
        return mFields.at(num);
    }
}