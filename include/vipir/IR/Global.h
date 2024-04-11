// Copyright 2024 solar-mist

/*
 * Globals are values that do not belong in a scope,
 * for example a function definition
*/

#ifndef VIPIR_IR_GLOBAL_H
#define VIPIR_IR_GLOBAL_H 1

#include "vipir/IR/Value.h"

namespace vipir
{
    class Global : public Value
    {
    friend class Module;
    public:
        Global(Module& module) : Value(module) { }
        virtual ~Global() { }
    };
}

#endif // VIPIR_IR_GLOBAL_H