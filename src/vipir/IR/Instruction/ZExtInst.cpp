// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/ZExtInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"

#include <format>

namespace vipir
{
    void ZExtInst::print(std::ostream& stream)
    {
        stream << std::format("%{} = zext {} -> {}", getName(mValueId), mValue->ident(), mType->getName());
    }

    std::string ZExtInst::ident() const
    {
        return std::format("{} %{}", mType->getName(), getName(mValueId));
    }

    void ZExtInst::doConstantFold()
    {
    }

    std::vector<std::reference_wrapper<Value*> > ZExtInst::getOperands()
    {
        return {mValue};
    }

    
    void ZExtInst::emit(lir::Builder& builder)
    {
        mValue->lateEmit(builder);
        lir::OperandPtr value = mValue->getEmittedValue();
        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());

        if (dynamic_cast<lir::Immediate*>(value.get()))
        {
            mEmittedValue = value->clone();
        }
        else
        {
            if (mValue->getType()->isBooleanType())
            {
                lir::OperandPtr vreg2 = std::make_unique<lir::VirtualReg>(mVReg, mValue->getType()->getOperandSize());
                builder.addValue(std::make_unique<lir::Move>(vreg2->clone(), std::move(value)));
                value = std::move(vreg2);
            }
            
            builder.addValue(std::make_unique<lir::MoveZX>(vreg->clone(), std::move(value)));

            mEmittedValue = std::move(vreg);
        }
    }


    ZExtInst::ZExtInst(BasicBlock* parent, Value* ptr, Type* destType)
        : Instruction(parent->getModule(), parent)
        , mValue(ptr)
        , mValueId(mModule.getNextValueId())
    {
        assert(destType->isIntegerType());
        assert(mValue->getType()->isIntegerType() || mValue->getType()->isBooleanType());
        
        if (mValue->getType()->isIntegerType())
        {
            assert(mValue->getType()->getSizeInBits() < destType->getSizeInBits());
        }

        mType = destType;
    }
}