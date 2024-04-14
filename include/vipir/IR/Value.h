// Copyright 2024 solar-mist

/*
 * A value is the base class for every statement or
 * expression in vipir
*/

#ifndef VIPIR_IR_VALUE_H
#define VIPIR_IR_VALUE_H 1

#include "vipir/Type/Type.h"

#include "vipir/MC/Builder.h"

#include "vasm/instruction/Operand.h"

#include <ostream>

namespace vipir
{
    class Module;
    
    class Value
    {
    friend class BasicBlock;
    friend class Function;
    public:
        Value(Module& module) : mModule(module), mType(Type::GetVoidType()) { }
        virtual ~Value() { }

        Module& getModule() { return mModule; }
        Type* getType() { return mType; }
        virtual std::vector<Value*> getOperands() { return std::vector<Value*>(); }

        virtual void print(std::ostream& stream) = 0;
        virtual std::string ident() const = 0;

        virtual instruction::OperandPtr& getEmittedValue() { return mEmittedValue; }
    
    protected:
        Module& mModule;
        Type* mType;
        
        instruction::OperandPtr mEmittedValue;

        bool requiresRegister{false};
        std::pair<int, int> mInterval{-1,-1};
        int mRegisterID;

        virtual void emit(MC::Builder& builder) = 0;
    };
}

#endif // VIPIR_IR_VALUE_H