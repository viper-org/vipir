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

    std::vector<Value*> RetInst::getOperands()
    {
        if (mReturnValue) return {mReturnValue};
        else return std::vector<Value*>();
    }

    std::string RetInst::ident() const
    {
        return "%undef";
    }

    void RetInst::emit(MC::Builder& builder)
    {
        if (mReturnValue)
        {
            instruction::OperandPtr returnValue = mReturnValue->getEmittedValue();
            if (returnValue)
            {
                instruction::Register* returnRegister = dynamic_cast<instruction::Register*>(returnValue.get());
                if (!returnRegister || returnRegister->getID() != mModule.abi()->getReturnRegister())
                {
                    instruction::OperandPtr reg = std::make_unique<instruction::Register>(mModule.abi()->getReturnRegister(), mReturnValue->getType()->getOperandSize());
                    builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(reg), std::move(returnValue)));
                }
            }
        }

        if (mParent->getParent()->usesStack())
        {
            builder.addValue(std::make_unique<instruction::LeaveInstruction>());
        }
        builder.addValue(std::make_unique<instruction::RetInstruction>());
    }

    void RetInst::emit2(lir::Builder& builder)
    {
        if (mReturnValue)
        {
            mReturnValue->lateEmit(builder);

            lir::OperandPtr returnRegister = std::make_unique<lir::PhysicalReg>(mModule.abi()->getReturnRegister(), mReturnValue->getType()->getOperandSize());
            builder.addValue(std::make_unique<lir::Move>(std::move(returnRegister), mReturnValue->getEmittedValue2()));
        }
        builder.addValue(std::make_unique<lir::Ret>());
    }

    RetInst::RetInst(BasicBlock* parent, Value* returnValue)
        : Instruction(parent->getModule(), parent)
        , mReturnValue(returnValue)
    {
        mRequiresVReg = false;
        if (!mReturnValue)
        {
            assert(mParent->getParent()->getFunctionType()->getReturnType() == Type::GetVoidType());
        }
        else
        {
            assert(mReturnValue->getType() == mParent->getParent()->getFunctionType()->getReturnType());
        }
    }
}