// Copyright 2023 solar-mist


#include "vipir/IR/Constant/ConstantInt.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    uint64_t ConstantInt::getValue() const
    {
        return mValue;
    }

    void ConstantInt::print(std::ostream& stream) const
    {
        stream << std::format("%{} = {} {}", mName, mType->getName(), mValue);
    }
    
    std::string ConstantInt::ident() const
    {
        return std::format("{} %{}", mType->getName(), mName);
    }

    bool ConstantInt::requiresRegister() const
    {
        return mColor != -1; // We only need a register if one has been precolored(such as a parameter)
    }

    std::vector<ValueId> ConstantInt::getOperands()
    {
        return {};
    }


    void ConstantInt::emit(std::vector<instruction::ValuePtr>& values)
    {
        if (!mRegister.empty())
        {
            values.push_back(std::make_unique<instruction::MovInstruction>(instruction::Register::Get(mRegister), std::make_unique<instruction::Immediate>(mValue), codegen::OperandSize::None));
            mEmittedValue = instruction::Register::Get(mRegister);
        }
        else
        {
            mEmittedValue = std::make_unique<instruction::Immediate>(mValue);
        }
    }


    ConstantInt::ConstantInt(BasicBlock* parent, ValueId id, uint64_t value, Type* type, std::string name)
        : Constant(parent->getParent()->getModule(), id)
        , mValue(value)
        , mName(name)
    {
        setType(type);
    }
}