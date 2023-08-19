// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/CallInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"
#include "vipir/Module.h"

#include <vasm/instruction/operand/Register.h>
#include <vasm/instruction/singleOperandInstruction/CallInstruction.h>

#include <format>

namespace vipir
{
    void CallInst::print(std::ostream& stream) const
    {
        stream << std::format("%{} = call {} {}()", mName, mType->getName(), mModule.getGlobals().at(mCallee)->ident());
    }

    std::string CallInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), mName);
    }

    bool CallInst::requiresRegister() const
    {
        return true;
    }

    std::vector<ValueId> CallInst::getOperands()
    {
        return {mCallee};
    }

    void CallInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        instruction::OperandPtr callee = mModule.getGlobalEmittedValue(mCallee);

        values.push_back(std::make_unique<instruction::CallInstruction>(std::move(callee)));

        mEmittedValue = instruction::Register::Get(mRegister);
    }

    CallInst::CallInst(BasicBlock* parent, ValueId id, std::string name, Value* callee)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mName(name)
        , mCallee(callee->getID())
    {
        mType = static_cast<Function*>(mModule.getGlobals().at(mCallee).get())->getReturnType();
        mColor = 0; // EAX
    }
}