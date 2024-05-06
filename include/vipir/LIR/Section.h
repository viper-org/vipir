// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_SECTION_H
#define VIPIR_LIR_SECTION_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        enum class SectionType
        {
            Code,
            Data
        };

        class Section : public Value
        {
        public:
            Section(SectionType type);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;
        
        private:
            SectionType mType;
        };
    }
}

#endif // VIPIR_LIR_SECTION_H