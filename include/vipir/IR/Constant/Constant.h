// Copyright 2023 solar-mist

/*
 * Constants are literal values such as integers or
 * strings
*/

#ifndef VIPIR_IR_CONSTANT_H
#define VIPIR_IR_CONSTANT_H 1
#include "vipir/IR/Value.h"

namespace vipir
{
    class Constant : public Value
    {
    public:
        Constant(Module& module, ValueId id) : Value(module, id) {}
    };
}

#endif // VIPIR_IR_CONSTANT_H