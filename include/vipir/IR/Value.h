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

#include <cassert>
#include <ostream>
#include <vector>

namespace vipir
{
    class Module;

    class RetInst;
    class StoreInst;
    class CallInst;

    class BasicBlock;
    class Function;
    
    using ValueId = int;

    class Value
    {
    friend class BasicBlock;
    friend class Function;
    friend class RetInst;
    friend class StoreInst;
    friend class CallInst;
    public:
        Value(Module& module, ValueId id) : mModule(module), mId(id) {}
        virtual ~Value() {}

        Type* getType() const { return mType; }
        ValueId getID() const { return mId; }
        void setType(Type* newType) { mType = newType; }

        void setRegister(std::string regName) { mRegister = regName; }
        virtual bool requiresRegister() const = 0;
        virtual std::vector<ValueId> getOperands() = 0;

        virtual void print(std::ostream& stream) const = 0;
        virtual std::string ident() const = 0;

    protected:
        Module& mModule;
        Type* mType;
        ValueId mId;
        instruction::OperandPtr mEmittedValue;

        std::vector<std::pair<ValueId, bool>> mEdges;
        int mColor{-1};
        std::string mRegister;

        virtual void emit(std::vector<instruction::ValuePtr>& values) = 0;
        virtual instruction::OperandPtr getEmittedValue()
        {
            assert(mEmittedValue != nullptr);
            return std::move(mEmittedValue);
        }
    };
}

#endif // VIPIR_IR_VALUE_H