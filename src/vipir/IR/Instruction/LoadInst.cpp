// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vipir/IR/Instruction/Instruction.h"
#include "vasm/instruction/operand/Memory.h"

#include <format>

namespace vipir
{
    void LoadInst::print(std::ostream& stream) const
    {
        stream << std::format("%{} = load {}", mName, mParent->getParent()->getValue(mPtr)->ident());
    }

    std::string LoadInst::ident() const
    {
        return std::format("%{}", mName);
    }

    bool LoadInst::requiresRegister() const
    {
        return true;
    }

    std::vector<ValueId> LoadInst::getOperands()
    {
        return {mPtr};
    }

    void LoadInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        instruction::OperandPtr ptrOperand = mParent->getEmittedValue(mPtr);

        values.emplace_back(std::make_unique<instruction::MovInstruction>(instruction::Register::Get(mRegister), std::move(ptrOperand), codegen::OperandSize::None));

        mEmittedValue = instruction::Register::Get(mRegister);
    }

    LoadInst::LoadInst(BasicBlock* parent, ValueId id, Value* ptr, std::string name)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mPtr(ptr->getID())
        , mName(name)
    {
        setType(static_cast<AllocaInst*>(mParent->getParent()->getValue(mPtr))->getAllocatedType());
    }
}