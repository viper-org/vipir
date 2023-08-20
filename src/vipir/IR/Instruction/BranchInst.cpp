// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/BranchInst.h"
#include "vasm/instruction/singleOperandInstruction/JmpInstruction.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include <format>

namespace vipir
{
    void BranchInst::print(std::ostream& stream) const
    {
        stream << std::format("br {}", mDestination->ident());
    }

    std::string BranchInst::ident() const
    {
        return "%undef";
    }

    bool BranchInst::requiresRegister() const
    {
        return false;
    }

    std::vector<ValueId> BranchInst::getOperands()
    {
        return {};
    }


    void BranchInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        values.emplace_back(std::make_unique<instruction::JmpInstruction>(mParent->getParent()->getBasicBlockEmittedValue(mDestination)));
    }


    BranchInst::BranchInst(BasicBlock* parent, ValueId id, BasicBlock* destination)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mDestination(destination)
    {
    }
}