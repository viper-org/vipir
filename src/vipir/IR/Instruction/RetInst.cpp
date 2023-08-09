// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/RetInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

namespace vipir
{
    void RetInst::print(std::ostream& stream) const
    {
        stream << "ret void\n";
    }

    std::string RetInst::ident() const
    {
        return "%undef";
    }


    void RetInst::emit(std::ostream& stream) const
    {
        stream << "ret";
    }


    RetInst::RetInst(BasicBlock* parent)
        :Instruction(parent->getParent()->getModule())
    {
    }
}