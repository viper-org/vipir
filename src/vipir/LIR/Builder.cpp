// Copyright 2024 solar-mist

#include "vipir/LIR/Builder.h"

namespace vipir
{
    namespace lir
    {
        void Builder::addValue(ValuePtr value)
        {
            mValues.push_back(std::move(value));
        }

        std::vector<Builder::ValuePtr>& Builder::getValues()
        {
            return mValues;
        }
    }
}