// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/RetInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vasm/instruction/NoOperandInstruction.h"

namespace vipir
{
    void RetInst::print(std::ostream& stream)
    {
        stream << "ret void";
    }

    void RetInst::emit(MC::Builder& builder)
    {
        builder.addValue(std::make_unique<instruction::LeaveInstruction>());
        builder.addValue(std::make_unique<instruction::RetInstruction>());
    }

    RetInst::RetInst(BasicBlock* parent, Value* returnValue)
        : Instruction(parent->getModule(), parent)
        , mReturnValue(returnValue)
    {
    }
}