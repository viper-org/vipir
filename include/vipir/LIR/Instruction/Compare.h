// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_INSTRUCTION_COMPARE_H
#define VIPIR_LIR_INSTRUCTION_COMPARE_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class Compare : public Value
        {
        friend class opt::Peephole;
        public:
            Compare(OperandPtr left, CMP::Operator op, OperandPtr right);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

        private:
            OperandPtr mLeft;
            CMP::Operator mOperator;
            OperandPtr mRight;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_COMPARE_H