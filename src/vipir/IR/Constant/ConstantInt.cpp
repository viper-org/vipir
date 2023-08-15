// Copyright 2023 solar-mist


#include "vipir/IR/Constant/ConstantInt.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/operand/Immediate.h"

#include <iostream>
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
        return false;
    }

    std::vector<ValueId> ConstantInt::getOperands()
    {
        return {};
    }


    void ConstantInt::emit(std::vector<instruction::ValuePtr>& values)
    {
        std::string regName;
        switch(mType->getSizeInBits())
        {
            case 8:
                regName = "al";
                break;
            case 16:
                regName = "ax";
                break;
            case 32:
                regName = "eax";
                break;
            case 64:
                regName = "rax";
                break;
        }
        mEmittedValue = std::make_unique<instruction::Immediate>(mValue);
    }


    ConstantInt::ConstantInt(BasicBlock* parent, ValueId id, uint64_t value, Type* type, std::string name)
        : Constant(parent->getParent()->getModule(), id)
        , mValue(value)
        , mName(name)
    {
        setType(type);
    }
}