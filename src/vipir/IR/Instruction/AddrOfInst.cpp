// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/AddrOfInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Type/PointerType.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"

#include <format>

namespace vipir
{
    void AddrOfInst::print(std::ostream& stream) const
    {
        stream << std::format("%{} = addr {}", mName, mParent->getParent()->getValue(mPtr)->ident());
    }

    std::string AddrOfInst::ident() const
    {
        return std::format("%{}", mName);
    }

    bool AddrOfInst::requiresRegister() const
    {
        return true;
    }

    std::vector<ValueId> AddrOfInst::getOperands()
    {
        return {mPtr};
    }


    void AddrOfInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        instruction::OperandPtr ptr = mParent->getEmittedValue(mPtr);

        values.emplace_back(std::make_unique<instruction::LeaInstruction>(instruction::Register::Get(mRegister), std::move(ptr), codegen::OperandSize::None));

        mEmittedValue = instruction::Register::Get(mRegister);
    }


    AddrOfInst::AddrOfInst(BasicBlock* parent, ValueId id, AllocaInst* ptr, std::string name)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mPtr(ptr->getID())
        , mName(name)
    {
        mType = PointerType::Get(ptr->getAllocatedType());
    }
}