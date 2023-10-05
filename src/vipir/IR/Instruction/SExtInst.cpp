// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/SExtInst.h"
#include "vasm/instruction/operand/Register.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Type/IntegerType.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    void SExtInst::print(std::ostream& stream) const
    {
        stream << std::format("%{} = sext {} to {}", mName, mParent->getParent()->getValue(mSource)->ident(), mDest->getName());
    }

    std::string SExtInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), mName);
    }

    bool SExtInst::requiresRegister() const
    {
        return true;
    }

    std::vector<ValueId> SExtInst::getOperands()
    {
        return {mSource};
    }

    void SExtInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        instruction::OperandPtr source = mParent->getEmittedValue(mSource);

        values.emplace_back(std::make_unique<instruction::MovSXInstruction>(instruction::Register::Get(mRegister), source->clone(), codegen::OperandSize::None));

        mEmittedValue = instruction::Register::Get(mRegister);
    }

    SExtInst::SExtInst(BasicBlock* parent, ValueId id, Value* source, Type* dest, std::string name)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mSource(source->getID())
        , mDest(dest)
        , mName(name)
    {
        assert(static_cast<IntegerType*>(dest));
        mType = dest;
    }
}