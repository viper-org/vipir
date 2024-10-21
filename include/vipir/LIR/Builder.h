// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_BUILDER_H
#define VIPIR_LIR_BUILDER_H 1

#include "vipir/LIR/Value.h"
#include "vipir/LIR/Section.h"

#include <memory>

namespace vipir
{
    namespace lir
    {
        using ValuePtr = std::unique_ptr<Value>;
        class Builder
        {
        public:
            void addValue(ValuePtr value);
            void insertValue(ValuePtr value, int position);
            void setSection(SectionType sect);

            int getPosition();
            int getInsertsBefore(int position);

            std::vector<ValuePtr>& getValues();

        private:
            std::vector<ValuePtr> mValues;

            std::vector<int> mInserts;
        };
    }
}

#endif // VIPIR_LIR_BUILDER_H