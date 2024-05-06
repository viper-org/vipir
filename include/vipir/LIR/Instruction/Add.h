// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_INSTRUCTION_ADD_H
#define VIPIR_LIR_INSTRUCTION_ADD_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class Add : public Value
        {
        public:
            Add(OperandPtr dest, OperandPtr left, OperandPtr right);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

        private:
            OperandPtr mDest;
            OperandPtr mLeft;
            OperandPtr mRight;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_ADD_H