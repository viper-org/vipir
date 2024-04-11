// Copyright 2024 solar-mist

#ifndef VIPIR_MC_BUILDER_H
#define VIPIR_MC_BUILDER_H 1

#include "vasm/instruction/Value.h"

namespace vipir
{
    namespace MC
    {
        class Builder
        {
        public:
            Builder();

            void addValue(instruction::ValuePtr value);

            std::vector<instruction::ValuePtr>& getValues();

        private:
            std::vector<instruction::ValuePtr> mValues;
        };
    }
}

#endif // VIPIR_MC_BUILDER_H