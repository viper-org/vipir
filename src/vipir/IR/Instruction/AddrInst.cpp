// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/LoadAddress.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Relative.h"
#include "vasm/instruction/operand/Label.h"

#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    void AddrInst::print(std::ostream& stream)
    {
        stream << std::format("addr {} %{}, {}", mType->getName(), getName(mValueId), mPtr->ident());
    }

    Value* AddrInst::getPointer()
    {
        return mPtr;
    }

    std::string AddrInst::ident() const
    {
        return std::format("%{}", getName(mValueId));
    }

    std::vector<std::reference_wrapper<Value*> > AddrInst::getOperands()
    {
        return { mPtr };
    }

    void AddrInst::doConstantFold()
    {
    }

    void AddrInst::cleanup()
    {
        if (auto alloca = dynamic_cast<AllocaInst*>(mPtr))
        {
            alloca->forceMemoryCount()--;
        }
    }

    void AddrInst::emit(lir::Builder& builder)
    {
        mPtr->lateEmit(builder);

        lir::OperandPtr ptr = mPtr->getEmittedValue();
        if (dynamic_cast<AllocaInst*>(mPtr))
        {
            mEmittedValue = std::move(ptr);
            return;
        }

        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
        builder.addValue(std::make_unique<lir::LoadAddress>(vreg->clone(), std::move(ptr)));

        mEmittedValue = std::move(vreg);
    }

    AddrInst::AddrInst(BasicBlock* parent, Value* ptr)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mValueId(mModule.getNextValueId())
    {
        mType = mPtr->getType();

        if (auto alloca = dynamic_cast<AllocaInst*>(mPtr))
        {
            mRequiresVReg = false;
            alloca->forceMemoryCount()++;
        }
        if (auto func = dynamic_cast<Function*>(mPtr))
        {
            mType = Type::GetPointerType(mType);
        }
    }
}