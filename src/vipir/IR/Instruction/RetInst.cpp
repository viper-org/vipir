// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/RetInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vasm/instruction/operand/Register.h"

#include "vasm/instruction/NoOperandInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

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
        return {mReturnValue};
    }

    std::string RetInst::ident() const
    {
        return "%undef";
    }

    void RetInst::emit(MC::Builder& builder)
    {
        if (mReturnValue)
        {
            instruction::OperandPtr& returnValue = mReturnValue->getEmittedValue();
            if (returnValue)
            {
                instruction::Register* returnRegister = dynamic_cast<instruction::Register*>(returnValue.get());
                if (!returnRegister || returnRegister->getID() != 0) // eax
                {
                    instruction::OperandPtr reg = std::make_unique<instruction::Register>(0, mReturnValue->getType()->getOperandSize());
                    builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(reg), std::move(returnValue)));
                }
            }
        }

        builder.addValue(std::make_unique<instruction::LeaveInstruction>());
        builder.addValue(std::make_unique<instruction::RetInstruction>());
    }

    RetInst::RetInst(BasicBlock* parent, Value* returnValue)
        : Instruction(parent->getModule(), parent)
        , mReturnValue(returnValue)
    {
    }
}