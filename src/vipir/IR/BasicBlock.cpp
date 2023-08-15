// Copyright 2023 solar-mist


#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/IR/Instruction/Instruction.h"

#include "vipir/Module.h"

#include "vasm/instruction/Label.h"

#include <format>

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

    const std::vector<InstructionPtr>& BasicBlock::getInstructionList() const
    {
        return mInstructionList;
    }

    void BasicBlock::insertInstruction(Instruction* instruction)
    {
        mInstructionList.push_back(InstructionPtr(instruction));
    }

    void BasicBlock::print(std::ostream& stream) const
    {
        stream << std::format("{}:\n", mName);
        for (const InstructionPtr& instruction : mInstructionList)
        {
            stream << "\t";
            instruction->print(stream);
            stream << "\n";
        }
    }

    std::string BasicBlock::ident() const
    {
        return std::format("%{}", mName);
    }


    instruction::OperandPtr BasicBlock::emit(std::vector<instruction::ValuePtr>& values) const
    {
        values.emplace_back(std::make_unique<instruction::Label>(mName));
        for (const InstructionPtr& instruction : mInstructionList)
        {
            instruction->emit(values);
        }
        return nullptr;
    }


    BasicBlock::BasicBlock(std::string name, Function* parent)
        : Value(parent->getModule())
        , mName(std::move(name))
        , mParent(parent)
        , mBranches(0)
    {
    }
}