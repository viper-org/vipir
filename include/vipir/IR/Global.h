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
    enum class OptimizationLevel;

    class Global : public Value
    {
    friend class Module;
    public:
        Global(Module& module) : Value(module, 0) {}

        virtual void optimize(OptimizationLevel level) = 0;
    };
    using GlobalPtr = std::unique_ptr<Global>;
}

#endif // VIPIR_IR_GLOBAL_H