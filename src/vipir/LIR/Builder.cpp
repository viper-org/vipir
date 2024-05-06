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

        void Builder::setSection(SectionType sect)
        {
            mValues.push_back(std::make_unique<Section>(sect));
        }

        std::vector<Builder::ValuePtr>& Builder::getValues()
        {
            return mValues;
        }
    }
}