// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/RetInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include <format>

namespace vipir
{
    Value* RetInst::getReturnValue() const
    {
        return mReturnValue;
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


    void RetInst::emit(std::ostream& stream) const
    {
        if (mReturnValue)
        {
            mReturnValue->emit(stream);
            stream << "\n\t";
        }
        stream << "ret";
    }


    RetInst::RetInst(BasicBlock* parent, Value* returnValue)
        : Instruction(parent->getParent()->getModule())
        , mReturnValue(returnValue)
    {
    }
}