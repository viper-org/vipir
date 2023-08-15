// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/RetInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/NoOperandInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/codegen/Opcodes.h"

#include <format>

namespace vipir
{
    Value* RetInst::getReturnValue() const
    {
        return mReturnValue.get();
    }

    Type* RetInst::getReturnType() const
    {
        if (!mReturnValue)
        {
            
        }
        return mReturnValue->getType();
    }

    void RetInst::print(std::ostream& stream) const
    {
        if (mReturnValue)
        {
            mReturnValue->print(stream);
            stream << std::format("ret {}\n", mReturnValue->ident());
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


    instruction::OperandPtr RetInst::emit(std::vector<instruction::ValuePtr>& values) const
    {
        if (mReturnValue)
        {
            instruction::OperandPtr returnValue = mReturnValue->emit(values);
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
        values.emplace_back(std::make_unique<instruction::RetInstruction>());
        return nullptr;
    }


    RetInst::RetInst(BasicBlock* parent, Value* returnValue)
        : Instruction(parent->getParent()->getModule())
        , mReturnValue(returnValue)
    {
    }
}