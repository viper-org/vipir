// Copyright 2023 solar-mist


#include "vipir/IR/Constant/ConstantInt.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include <format>

namespace vipir
{
    uint64_t ConstantInt::getValue() const
    {
        return mValue;
    }

    void ConstantInt::print(std::ostream& stream) const
    {
        stream << std::format("%{} = {} {}\n\t", mName, mType->getName(), mValue);
    }
    
    std::string ConstantInt::ident() const
    {
        return std::format("%{}", mName);
    }


    void ConstantInt::emit(std::ostream& stream) const
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
        stream << std::format("mov {}, {}", regName, mValue);
    }


    ConstantInt::ConstantInt(BasicBlock* parent, uint64_t value, Type* type, std::string name)
        : Constant(parent->getParent()->getModule())
        , mValue(value)
        , mName(name)
    {
        setType(type);
    }
}