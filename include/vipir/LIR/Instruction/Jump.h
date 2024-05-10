// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_INSTRUCTION_JUMP_H
#define VIPIR_LIR_INSTRUCTION_JUMP_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class Jump : public Value
        {
        friend class opt::Peephole;
        public:
            Jump(OperandPtr dest);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

            std::vector<std::reference_wrapper<OperandPtr> > getInputOperands() override;

        private:
            OperandPtr mDest;
        };

        class CondJump : public Value
        {
        friend class opt::Peephole;
        public:
            CondJump(OperandPtr dest, OperandPtr condition);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

            std::vector<std::reference_wrapper<OperandPtr> > getInputOperands() override;

        private:
            OperandPtr mDest;
            OperandPtr mCondition;
        };


        class Call : public Value
        {
        friend class opt::Peephole;
        public:
            Call(OperandPtr dest);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

            std::vector<std::reference_wrapper<OperandPtr> > getInputOperands() override;

        private:
            OperandPtr mDest;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_JUMP_H