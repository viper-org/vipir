// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_INSTRUCTION_ARITHMETIC_H
#define VIPIR_LIR_INSTRUCTION_ARITHMETIC_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class BinaryArithmetic : public Value
        {
        friend class opt::Peephole;
        public:
            enum class Operator
            {
                Add, Sub,

                BWAnd, BWOr, BWXor
            };

            BinaryArithmetic(OperandPtr left, Operator op, OperandPtr right);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

        private:
            OperandPtr mLeft;
            Operator mOperator;
            OperandPtr mRight;
        };


        class UnaryArithmetic : public Value
        {
        friend class opt::Peephole;
        public:
            enum class Operator
            {
                Not,
                Neg
            };

            UnaryArithmetic(OperandPtr operand, Operator op);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

        private:
            OperandPtr mOperand;
            Operator mOperator;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_ARITHMETIC_H