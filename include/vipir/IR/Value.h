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
    class StoreInst;

    class BasicBlock;
    
    using ValueId = int;

    class Value
    {
    friend class BasicBlock;
    friend class RetInst;
    friend class StoreInst;
    public:
        Value(Module& module, ValueId id) : mModule(module), mId(id) {}
        virtual ~Value() {}

        Type* getType() const { return mType; }
        ValueId getID() const { return mId; }
        void setType(Type* newType) { mType = newType; }

        virtual void print(std::ostream& stream) const = 0;
        virtual std::string ident() const = 0;

    protected:
        Module& mModule;
        Type* mType;
        ValueId mId;
        instruction::OperandPtr mEmittedValue;

        virtual void emit(std::vector<instruction::ValuePtr>& values) = 0;
        instruction::OperandPtr getEmittedValue() { return std::move(mEmittedValue); }
    };
}

#endif // VIPIR_IR_VALUE_H