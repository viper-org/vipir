// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_INSTRUCTION_ADD_H
#define VIPIR_LIR_INSTRUCTION_ADD_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class Arithmetic : public Value
        {
        public:
            enum class Operator
            {
                Add, Sub
            };

            Arithmetic(OperandPtr left, Operator op, OperandPtr right);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

        private:
            OperandPtr mLeft;
            Operator mOperator;
            OperandPtr mRight;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_ADD_H