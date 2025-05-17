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
#include "vipir/Optimizer/DCE/DeadCodeElimination.h"
#include "vipir/Optimizer/AA/AliasAnalysis.h"
#include "vipir/Optimizer/Mem2Reg/Dom.h"
#include "vipir/Optimizer/Mem2Reg/Mem2Reg.h"

#include "vipir/LIR/Builder.h"

#include "vasm/instruction/Operand.h"

#include <format>
#include <ostream>

namespace vipir
{
    class Module;
    class DIType;
    
    class Value
    {
    friend class Module;
    friend class BasicBlock;
    friend class Function;
    friend class DIBuilder;
    friend class opt::RegAlloc;
    friend class opt::DeadCodeEliminator;
    friend class opt::AliasAnalyzer;
    public:
        Value(Module& module) : mModule(module), mType(Type::GetVoidType()), mRegisterSmashesDone(false), mId(0x69696969) { }
        virtual ~Value() { }

        Module& getModule() { return mModule; }
        Type* getType() { return mType; }
        virtual std::vector<std::reference_wrapper<Value*> > getOperands() { return std::vector<std::reference_wrapper<Value*> >(); }
        virtual bool isConstant() const { return false; }

        virtual void print(std::ostream& stream) = 0;
        virtual std::string ident() const = 0;

        bool requiresVReg() const { return mRequiresVReg; }
        void setPreferredRegisterID(int id) { mPreferredRegisterID = id; }

        std::string getName(int valueId) const { if (mVReg) return std::format("VREG{}", mVReg->getId()); else return std::format("{}", valueId); }

        virtual void doConstantFold() = 0;
        bool isConstantFolded() { return mIsConstantFolded; }
        std::uintmax_t getConstantFoldedValue() { return mConstantFoldedValue; }

        virtual lir::OperandPtr getEmittedValue() { return mEmittedValue->clone(); }
        virtual void lateEmit(lir::Builder& builder) { }
        virtual std::vector<int> getRegisterSmashes() { return std::vector<int>(); }

        virtual bool isCritical() { return false; }
        virtual void cleanup() {} // In case cleanup needs to be done when instruction is optimized away


        int getLine() { return mLine; }
        int getCol() { return mCol; }
    
    protected:
        Module& mModule;
        Type* mType;
        
        lir::OperandPtr mEmittedValue;

        std::pair<int, int> mInterval{-1,-1};
        int mId;

        bool mIsConstantFolded{ false };
        std::uintmax_t mConstantFoldedValue{ 0 };

        opt::VReg* mVReg{0};
        bool mRequiresVReg{true};
        std::vector<opt::VReg*> mDisallowedVRegs;
        std::vector<int> mDisallowedRegIds;
        bool mRegisterSmashesDone;
        int mPreferredRegisterID{ -1 };

        bool mMarked{ false };

        DIType* mDebugType { nullptr };
        int mLine{ 0 };
        int mCol{ 0 };

        virtual void emit(lir::Builder& builder) = 0;
    };
}

#endif // VIPIR_IR_VALUE_H