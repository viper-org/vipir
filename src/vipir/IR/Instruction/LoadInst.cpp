// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/LIR/Instruction/Move.h"
#include "vipir/Module.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/Relative.h"

#include "vipir/Type/PointerType.h"

#include <cassert>
#include <format>

namespace vipir
{
    void LoadInst::print(std::ostream& stream)
    {
        stream << std::format("load {} %{}, {}", mType->getName(), getName(mValueId), mPtr->ident());
    }

    Value* LoadInst::getPointer()
    {
        return mPtr;
    }
    std::vector<std::reference_wrapper<Value*> > LoadInst::getOperands()
    {
        return {mPtr};
    }

    std::string LoadInst::ident() const
    {
        return std::format("%{}", getName(mValueId));
    }

    void LoadInst::doConstantFold()
    {
    }


    void LoadInst::emit(lir::Builder& builder)
    {
        mPtr->lateEmit(builder);

        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
        lir::OperandPtr mem = mPtr->getEmittedValue();
        if (!dynamic_cast<AllocaInst*>(mPtr))
            mem = std::make_unique<lir::Memory>(mType->getOperandSize(), std::move(mem), std::nullopt, nullptr, std::nullopt);
        
        builder.addValue(std::make_unique<lir::Move>(vreg->clone(), std::move(mem)));
        mEmittedValue = std::move(vreg);
    }

    LoadInst::LoadInst(BasicBlock* parent, Value* ptr)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mValueId(mModule.getNextValueId())
    {
        assert(mPtr->getType()->isPointerType());
        mType = static_cast<PointerType*>(mPtr->getType())->getBaseType();
    }
}