// Copyright 2023 solar-mist

/*
 * Globals are values that do not belong to a specific
 * scope e.g. a function definition
*/

#ifndef VIPIR_IR_GLOBAL_H
#define VIPIR_IR_GLOBAL_H 1
#include "vipir/IR/Value.h"

namespace vipir
{
    class Global : public Value
    {
    public:
        Global(Module& module) : Value(module) {}
    };
}

#endif // VIPIR_IR_GLOBAL_H