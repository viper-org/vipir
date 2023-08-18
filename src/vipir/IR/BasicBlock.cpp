// Copyright 2023 solar-mist


#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vasm/instruction/Label.h"

#include <algorithm>
#include <format>
#include <iostream>

namespace vipir
{
    BasicBlock* BasicBlock::Create(std::string name, Function* parent)
    {
        if (name.empty())
        {
            name = std::to_string(parent->getInstructionCount()++);
        }

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

    std::vector<ValueId>& BasicBlock::getInstructionList()
    {
        return mValueList;
    }

    instruction::OperandPtr BasicBlock::getEmittedValue(ValueId id)
    {
        return mParent->getValue(id)->getEmittedValue();
    }

    void BasicBlock::insertValue(Value* value)
    {
        mValueList.push_back(value->getID());
    }

    void BasicBlock::eraseValue(ValueId value)
    {
        auto it = std::find(mValueList.begin(), mValueList.end(), value);

        if (it != mValueList.end())
        {
            mValueList.erase(it);
        }
    }

    void BasicBlock::print(std::ostream& stream) const
    {
        stream << std::format("{}:\n", mName);
        for (ValueId instruction : mValueList)
        {
            stream << "\t";
            mParent->getValue(instruction)->print(stream);
            stream << "\n";
        }
    }

    bool BasicBlock::requiresRegister() const
    {
        return false;
    }

    std::vector<ValueId> BasicBlock::getOperands()
    {
        return {};
    }

    std::string BasicBlock::ident() const
    {
        return std::format("%{}", mName);
    }


    void BasicBlock::emit(std::vector<instruction::ValuePtr>& values)
    {
        values.emplace_back(std::make_unique<instruction::Label>(".L" + mName));
        for (ValueId instruction : mValueList)
        {
            mParent->getValue(instruction)->emit(values);
        }
    }


    BasicBlock::BasicBlock(std::string name, Function* parent)
        : Value(parent->getModule(), parent->getInstructionCount()++)
        , mName(std::move(name))
        , mParent(parent)
        , mBranches(0)
    {
    }
}