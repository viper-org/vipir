// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/CallInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"
#include "vipir/Module.h"

#include "vipir/Type/VoidType.h"

#include <cassert>
#include <vasm/instruction/operand/Register.h>
#include <vasm/instruction/singleOperandInstruction/CallInstruction.h>
#include <vasm/instruction/twoOperandInstruction/MovInstruction.h>

#include <algorithm>
#include <format>

namespace vipir
{
    void CallInst::print(std::ostream& stream) const
    {
        stream << std::format("%{} = call {} {}(", mName, mType->getName(), mModule.getGlobals().at(mCallee)->ident());
        for (auto it = mParameters.begin(); it != mParameters.end(); it++)
        {
            stream << mParent->getParent()->getValue(*it)->ident();
            if ((it + 1) != mParameters.end())
            {
                stream << ", ";
            }
        }
        stream << ")";
    }

    std::string CallInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), mName);
    }

    bool CallInst::requiresRegister() const
    {
        return !dynamic_cast<VoidType*>(mType);
    }

    std::vector<ValueId> CallInst::getOperands()
    {
        std::vector<ValueId> operands = {mCallee};
        for (auto parameter : mParameters)
        {
            std::copy(mParent->getParent()->getValue(parameter)->getOperands().begin(), mParent->getParent()->getValue(parameter)->getOperands().end(), std::back_inserter(operands)); // In case parameters have their own operands
            operands.push_back(parameter);
        }
        return operands;
    }

    void CallInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        instruction::OperandPtr callee = mModule.getGlobalEmittedValue(mCallee);

        values.push_back(std::make_unique<instruction::CallInstruction>(std::move(callee)));

        if (mColor != 0) // EAX
        {
            values.push_back(std::make_unique<instruction::MovInstruction>(instruction::Register::Get(mRegister), instruction::Register::Get("eax"), codegen::OperandSize::None));
        }

        mEmittedValue = instruction::Register::Get(mRegister);
    }

    CallInst::CallInst(BasicBlock* parent, ValueId id, std::string name, Value* callee, const std::vector<Value*>& parameters)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mName(name)
        , mCallee(callee->getID())
    {
        Function* function = static_cast<Function*>(mModule.getGlobals().at(mCallee).get());

        std::array paramRegisters = { 5, 4, 2, 1 };
        int index = 0;
        std::transform(parameters.begin(), parameters.end(), std::back_inserter(mParameters), [&paramRegisters, &index, function](auto parameter) {
            assert(parameter->getType() == function->getArgument(index)->getType());
            parameter->mColor = paramRegisters[index++];
            return parameter->getID();
        });

        mType = function->getReturnType();
    }
}