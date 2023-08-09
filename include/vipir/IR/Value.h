// Copyright 2023 solar-mist

/*
 * A value is the base class for every statement or
 * expression in vipir
*/

#ifndef VIPIR_IR_VALUE_H
#define VIPIR_IR_VALUE_H 1

#include <ostream>

namespace vipir
{
    class Module;

    class Value
    {
    public:
        Value(Module& module) : mModule(module) {}
        virtual ~Value() {}

        virtual void print(std::ostream& stream) const = 0;
        virtual std::string ident() const = 0;
    protected:
        Module& mModule;
    };
}

#endif // VIPIR_IR_VALUE_H