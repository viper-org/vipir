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
                instruction::OperandPtr reg = instruction::Register::Get("eax"); // TODO: Get size from return value
                builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(reg), std::move(returnValue)));
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