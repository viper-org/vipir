// Copyright 2024 solar-mist

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/Label.h"

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

    Function* BasicBlock::getParent()
    {
        return mParent;
    }

    void BasicBlock::insertValue(Value* value)
    {
        mValueList.push_back(ValuePtr(value));
    }

    void BasicBlock::print(std::ostream& stream)
    {
        stream << std::format("{}:\n", mName);
        for (auto& value : mValueList)
        {
            stream << "\t";
            value->print(stream);
            stream << "\n";
        }
    }

    std::string BasicBlock::ident() const
    {
        return std::format("label %{}", mName);
    }

    void BasicBlock::emit(MC::Builder& builder)
    {
        builder.addValue(std::make_unique<instruction::Label>(mName));
        for (auto& value : mValueList)
        {
            value->emit(builder);
        }
    }


    BasicBlock::BasicBlock(std::string_view name, Function* parent)
        : Value(parent->getModule())
        , mName(name)
        , mParent(parent)
    {
    }
}