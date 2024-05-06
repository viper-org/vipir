// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_INSTRUCTION_ENTER_FUNC_H
#define VIPIR_LIR_INSTRUCTION_ENTER_FUNC_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class EnterFunc : public Value
        {
        public:
            EnterFunc(int stackSize);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;

        private:
            int mStackSize;
        };
    }
}

#endif // VIPIR_LIR_INSTRUCTION_ENTER_FUNC_H