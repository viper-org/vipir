// Copyright 2023 solar-mist

/*
 * A value is the base class for every statement or
 * expression in vipir
*/

#ifndef VIPIR_IR_VALUE_H
#define VIPIR_IR_VALUE_H 1

#include "vipir/Type/Type.h"

#include "vasm/instruction/Value.h"
#include "vasm/instruction/Operand.h"

#include <ostream>
#include <vector>

namespace vipir
{
    class Module;

    class RetInst;

    class Value
    {
    friend class RetInst;
    public:
        Value(Module& module) : mModule(module) {}
        virtual ~Value() {}

        Type* getType() const { return mType; }
        void setType(Type* newType) { mType = newType; }

        virtual void print(std::ostream& stream) const = 0;
        virtual std::string ident() const = 0;
    protected:
        Module& mModule;
        Type* mType;

        virtual instruction::OperandPtr emit(std::vector<instruction::ValuePtr>& values) const = 0;
    };
}

#endif // VIPIR_IR_VALUE_H