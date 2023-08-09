// Copyright 2023 solar-mist


#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include <format>

namespace vipir
{
    BasicBlock* BasicBlock::Create(std::string name, Function* parent)
    {
        if(name.empty())
            name = std::to_string(parent->getInstructionCount()++);

        BasicBlock* bb = new BasicBlock(std::move(name), parent);

        parent->insertBasicBlock(bb);

        return bb;
    }

    Function* BasicBlock::getParent() const
    {
        return mParent;
    }

    std::string_view BasicBlock::getName() const
    {
        return mName;
    }

    int BasicBlock::getNoBranches() const
    {
        return mBranches;
    }

    void BasicBlock::print(std::ostream& stream) const
    {
        stream << std::format("{}:\n", mName);
    }

    std::string BasicBlock::ident() const
    {
        return std::format("%{}", mName);
    }


    void BasicBlock::emit(std::ostream& stream) const
    {
        if (mBranches)
        {
            stream << std::format(".L{}:", mName);
        }
    }


    BasicBlock::BasicBlock(std::string name, Function* parent)
        :Value(parent->getModule()), mName(std::move(name)), mParent(parent), mBranches(0)
    {
    }
}