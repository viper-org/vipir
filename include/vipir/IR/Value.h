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

#include "vipir/LIR/Builder.h"

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
        Value(Module& module) : mModule(module), mType(Type::GetVoidType()), mRegisterSmashesDone(false), mId(0x69696969) { }
        virtual ~Value() { }

        Module& getModule() { return mModule; }
        Type* getType() { return mType; }
        virtual std::vector<Value*> getOperands() { return std::vector<Value*>(); }
        virtual bool isConstant() const { return false; }

        virtual void print(std::ostream& stream) = 0;
        virtual std::string ident() const = 0;

        bool requiresVReg() const { return mRequiresVReg; }

        std::string getName(int valueId) const { if (mVReg) return std::format("VREG{}", mVReg->getId()); else return std::format("{}", valueId); }

        virtual lir::OperandPtr getEmittedValue() { return mEmittedValue->clone(); }
        virtual void lateEmit(lir::Builder& builder) { }
        virtual std::vector<int> getRegisterSmashes() { return std::vector<int>(); }
    
    protected:
        Module& mModule;
        Type* mType;
        
        lir::OperandPtr mEmittedValue;

        std::pair<int, int> mInterval{-1,-1};
        int mId;

        opt::VReg* mVReg{0};
        bool mRequiresVReg{true};
        std::vector<opt::VReg*> mDisallowedVRegs;
        bool mRegisterSmashesDone;

        virtual void emit(lir::Builder& builder) = 0;
    };
}

#endif // VIPIR_IR_VALUE_H