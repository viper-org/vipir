// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_BUILDER_H
#define VIPIR_LIR_BUILDER_H 1

#include "vipir/LIR/Value.h"

#include <memory>

namespace vipir
{
    namespace lir
    {
        class Builder
        {
        using ValuePtr = std::unique_ptr<Value>;
        public:
            void addValue(ValuePtr value);

            std::vector<ValuePtr>& getValues();

        private:
            std::vector<ValuePtr> mValues;
        };
    }
}

#endif // VIPIR_LIR_BUILDER_H