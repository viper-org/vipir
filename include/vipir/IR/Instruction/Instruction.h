// Copyright 2024 solar-mist

/*
 * An instruction is the base class for everything in
 * BasicBlocks in vipir, e.g. Returns or Loads
*/

#ifndef VIPIR_IR_INSTRUCTION_INSTRUCTION_H
#define VIPIR_IR_INSTRUCTION_INSTRUCTION_H 1

#include "vipir/IR/Value.h"

namespace vipir
{
    class BasicBlock;

    class Instruction : public Value
    {
    friend class Module;
    public:
        Instruction(Module& module, BasicBlock* parent) : Value(module), mParent(parent) { }

        enum BinaryOperators
        {
            ADD, SUB
        };

    protected:
        BasicBlock* mParent;
    };
}

#endif // VIPIR_IR_INSTRUCTION_INSTRUCTION_H