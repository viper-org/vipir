// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_VALUE_H
#define VIPIR_LIR_VALUE_H 1

#include "vipir/LIR/Operand.h"

#include "vipir/MC/Builder.h"

#include <ostream>

namespace vipir
{
    namespace lir
    {
        class Value
        {
        public:
            virtual void print(std::ostream& stream) const = 0;

            virtual void emit(MC::Builder& builder) = 0;

            virtual std::vector<std::reference_wrapper<OperandPtr> > getOutputOperands() { return {}; }
            virtual std::vector<std::reference_wrapper<OperandPtr> > getInputOperands() { return {}; }
        };
    }
}

#endif // VIPIR_LIR_VALUE_H