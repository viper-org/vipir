// Copyright 2024 solar-mist

/*
 * A value is the base class for every statement or
 * expression in vipir
*/

#ifndef VIPIR_IR_VALUE_H
#define VIPIR_IR_VALUE_H 1

#include "vipir/Type/Type.h"

#include "vipir/MC/Builder.h"

#include "vipir/Optimizer/RegAlloc/VReg.h"

#include "vasm/instruction/Operand.h"

#include <format>
#include <ostream>

namespace vipir
{
    class Module;
    
    class Value
    {
    friend class Module;
    friend class BasicBlock;
    friend class Function;
    friend class opt::RegAlloc;
    public:
        Value(Module& module) : mModule(module), mType(Type::GetVoidType()) { }
        virtual ~Value() { }

        Module& getModule() { return mModule; }
        Type* getType() { return mType; }
        virtual std::vector<Value*> getOperands() { return std::vector<Value*>(); }
        virtual bool isConstant() const { return false; }

        virtual void print(std::ostream& stream) = 0;
        virtual std::string ident() const = 0;

        bool requiresVReg() const { return mRequiresVReg; }

        std::string getName(int valueId) const { if (mVReg) return std::format("VREG{}", mVReg->getId()); else return std::format("{}", valueId); }

        virtual instruction::OperandPtr getEmittedValue() { return mEmittedValue->clone(); }
    
    protected:
        Module& mModule;
        Type* mType;
        
        instruction::OperandPtr mEmittedValue;

        opt::VReg* mVReg{0};
        bool mRequiresVReg{true};

        std::pair<int, int> mInterval{-1,-1};

        virtual void emit(MC::Builder& builder) = 0;
    };
}

#endif // VIPIR_IR_VALUE_H