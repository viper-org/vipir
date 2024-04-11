// Copyright 2024 solar-mist

#include "vipir/IR/BasicBlock.h"
#include "vasm/instruction/Label.h"
#include "vipir/IR/Function.h"

#include <format>

namespace vipir
{
    static int basicBlockNumber = 0;

    BasicBlock* BasicBlock::Create(std::string_view name, Function* parent)
    {
        std::string basicBlockName = std::string(name);
        if (basicBlockName.empty())
        {
            basicBlockName = std::to_string(basicBlockNumber);
        }

        BasicBlock* basicBlock = new BasicBlock(std::move(basicBlockName), parent);
        
        parent->insertBasicBlock(basicBlock);

        return basicBlock;
    }

    void BasicBlock::print(std::ostream& stream)
    {
        stream << std::format("{}:\n", mName);
    }

    void BasicBlock::emit(MC::Builder& builder)
    {
        builder.addValue(std::make_unique<instruction::Label>(mName));
    }


    BasicBlock::BasicBlock(std::string_view name, Function* parent)
        : Value(parent->getModule())
        , mName(name)
        , mParent(parent)
    {
    }
}