// Copyright 2024 solar-mist

/*
 * A value is the base class for every statement or
 * expression in vipir
*/

#ifndef VIPIR_IR_VALUE_H
#define VIPIR_IR_VALUE_H 1

#include "vipir/MC/Builder.h"

#include <ostream>

namespace vipir
{
    class Module;
    
    class Value
    {
    public:
        Value(Module& module) : mModule(module) { }
        virtual ~Value() { }

        Module& getModule() { return mModule; }

        virtual void print(std::ostream& stream) = 0;
    
    protected:
        Module& mModule;

        virtual void emit(MC::Builder& builder) = 0;
    };
}

#endif // VIPIR_IR_VALUE_H