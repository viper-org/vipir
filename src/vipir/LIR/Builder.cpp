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
        
        void Builder::insertValue(ValuePtr value, int position)
        {
            mValues.insert(mValues.begin() + position, std::move(value));
            ++mInsertCount;
        }

        void Builder::setSection(SectionType sect)
        {
            mValues.push_back(std::make_unique<Section>(sect));
        }

        int Builder::getPosition()
        {
            return mValues.size();
        }

        int Builder::getInsertCount()
        {
            return mInsertCount;
        }

        std::vector<ValuePtr>& Builder::getValues()
        {
            return mValues;
        }
    }
}