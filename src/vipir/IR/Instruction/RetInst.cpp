// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/RetInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/NoOperandInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/codegen/Opcodes.h"

#include <cassert>
#include <format>

namespace vipir
{
    Value* RetInst::getReturnValue() const
    {
        return mParent->getParent()->getValue(mReturnValue);
    }

    Type* RetInst::getReturnType() const
    {
        return mParent->getParent()->getValue(mReturnValue)->getType();
    }

    void RetInst::print(std::ostream& stream) const
    {
        if (mReturnValue != -1)
        {
            stream << std::format("ret {}", mParent->getParent()->getValue(mReturnValue)->ident());
        }
        else
        {
            stream << "ret void\n";
        }
    }

    std::string RetInst::ident() const
    {
        return "%undef";
    }

    bool RetInst::requiresRegister() const
    {
        return false;
    }

    std::vector<ValueId> RetInst::getOperands()
    {
        return {mReturnValue};
    }


    void RetInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        if (mReturnValue != -1)
        {
            instruction::OperandPtr returnValue = mParent->getEmittedValue(mReturnValue);
            if (returnValue)
            {
                instruction::OperandPtr eax = std::make_unique<instruction::Register>(0, codegen::OperandSize::Long); // TODO: Get size properly
                if (instruction::Register* reg = dynamic_cast<instruction::Register*>(returnValue.get()))
                {
                    if (reg->getID() != 0) // EAX
                    {
                        values.push_back(std::make_unique<instruction::MovInstruction>(std::move(eax), std::move(returnValue), codegen::OperandSize::None));
                    }
                }
                else
                {
                    values.push_back(std::make_unique<instruction::MovInstruction>(std::move(eax), std::move(returnValue), codegen::OperandSize::None));
                }
            }
        }
        values.emplace_back(std::make_unique<instruction::LeaveInstruction>());
        values.emplace_back(std::make_unique<instruction::RetInstruction>());
    }


    RetInst::RetInst(BasicBlock* parent, ValueId id, Value* returnValue)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mReturnValue(returnValue ? returnValue->getID() : -1)
    {
        if (mReturnValue != -1)
        {
            assert(mParent->getParent()->getValue(mReturnValue)->getType() == mParent->getParent()->getReturnType());
            if (mParent->getParent()->getValue(mReturnValue)->mColor == -1)
            {
                mParent->getParent()->getValue(mReturnValue)->mColor = 0; // EAX
            }
        }
    }
}