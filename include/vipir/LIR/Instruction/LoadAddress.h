// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_INSTRUCTION_LOAD_ADDRESS_H
#define VIPIR_LIR_INSTRUCTION_LOAD_ADDRESS_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class LoadAddress : public Value
        {
        public:
            LoadAddress(OperandPtr left, OperandPtr right);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

        private:
            OperandPtr mLeft;
            OperandPtr mRight;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_LOAD_ADDRESS_H