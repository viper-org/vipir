// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/RetInst.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Ret.h"
#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/operand/Register.h"

#include "vasm/instruction/NoOperandInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <cassert>

namespace vipir
{
    void RetInst::print(std::ostream& stream)
    {
        stream << "ret ";
        if (mReturnValue)
        {
            stream << mReturnValue->ident();
        }
        else
        {
            stream << "void";
        }
    }

    std::vector<std::reference_wrapper<Value*> > RetInst::getOperands()
    {
        if (mReturnValue) return { mReturnValue };
        else return {};
    }

    bool RetInst::isCritical()
    {
        return true;
    }

    std::string RetInst::ident() const
    {
        return "%undef";
    }

    void RetInst::doConstantFold()
    {
    }

    void RetInst::emit(lir::Builder& builder)
    {
        mParent->endNode() = builder.getLastNode();

        if (mReturnValue)
        {
            mReturnValue->lateEmit(builder);

            lir::OperandPtr returnRegister = std::make_unique<lir::PhysicalReg>(mCallingConvention->getReturnRegister(), mReturnValue->getType()->getOperandSize());
            builder.addValue(std::make_unique<lir::Move>(std::move(returnRegister), mReturnValue->getEmittedValue()));
        }
        builder.addValue(std::make_unique<lir::Ret>(mParent->getParent()->usesStack(), mParent->getParent()->getCalleeSaved()));
    }

    RetInst::RetInst(BasicBlock* parent, Value* returnValue, const abi::CallingConvention* callingConvention)
            : Instruction(parent->getModule(), parent)
            , mCallingConvention(callingConvention)
            , mReturnValue(returnValue)
    {
        mRequiresVReg = false;
        if (!mReturnValue)
        {
            assert(mParent->getParent()->getFunctionType()->getReturnType() == Type::GetVoidType());
        }
        else
        {
            mReturnValue->setPreferredRegisterID(mCallingConvention->getReturnRegister());
            assert(mReturnValue->getType() == mParent->getParent()->getFunctionType()->getReturnType());
        }
    }
}