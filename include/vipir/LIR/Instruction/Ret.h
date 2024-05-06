// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_INSTRUCTION_RET_H
#define VIPIR_LIR_INSTRUCTION_RET_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class Ret : public Value
        {
        public:
            Ret();

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_RET_H