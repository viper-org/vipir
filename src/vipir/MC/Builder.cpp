// Copyright 2024 solar-mist

#include "vipir/MC/Builder.h"

namespace vipir
{
    namespace MC
    {
        Builder::Builder() {}

        void Builder::addValue(instruction::ValuePtr value)
        {
            mValues.push_back(std::move(value));
        }

        std::vector<instruction::ValuePtr>& Builder::getValues()
        {
            return mValues;
        }
    }
}