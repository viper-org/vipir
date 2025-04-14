// Copyright 2024 solar-mist

#include "vipir/LIR/Builder.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace vipir
{
    namespace lir
    {
        void Builder::addValue(ValuePtr value)
        {
            mValues.push_back(std::move(value));
        }

        void Builder::insertValue(ValuePtr value, Value* after)
        {
            auto it = std::find_if(mValues.begin(), mValues.end(), [after](const auto& value) {
                return value.get() == after;
            });
            assert(it != mValues.end());

            mValues.insert(it+1, std::move(value));
        }

        void Builder::setSection(SectionType sect)
        {
            mValues.push_back(std::make_unique<Section>(sect));
        }

        lir::Value* Builder::getLastNode()
        {
            return mValues.back().get();
        }

        int Builder::getPosition()
        {
            return mValues.size();
        }

        std::vector<ValuePtr>& Builder::getValues()
        {
            return mValues;
        }
    }
}