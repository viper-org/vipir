// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/StoreParamInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/LIR/Instruction/LoadAddress.h"
#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"
#include "vipir/LIR/Instruction/Arithmetic.h"

#include <format>

namespace vipir
{
    void StoreParamInst::print(std::ostream& stream)
    {
        stream << std::format("store {} -> PARAM{}", mValue->ident(), mParamIndex);
    }

    std::vector<std::reference_wrapper<Value*> > StoreParamInst::getOperands()
    {
        return { mValue };
    }

    std::vector<int> StoreParamInst::getRegisterSmashes()
    {
        return { mCallingConvention->getParameterRegister(mParamIndex) };
    }

    bool StoreParamInst::isCritical()
    {
        return mCall->isCritical();
    }

    std::string StoreParamInst::ident() const
    {
        return "%undef";
    }

    void StoreParamInst::doConstantFold()
    {
    }

    void StoreParamInst::emit(lir::Builder& builder)
    {
        mValue->lateEmit(builder);

        int regID = mCallingConvention->getParameterRegister(mParamIndex);
        lir::OperandPtr ptr;
        lir::OperandPtr value = mValue->getEmittedValue();

        // align the stack if it is made unaligned by the passing of parameters
        lir::OperandPtr stack = std::make_unique<lir::PhysicalReg>(mModule.abi()->getStackArgumentRegister(), codegen::OperandSize::Quad);
        if (mAlignStack)
        {
            lir::OperandPtr alignment = std::make_unique<lir::Immediate>(8);
            builder.addValue(std::make_unique<lir::BinaryArithmetic>(stack->clone(), lir::BinaryArithmetic::Operator::Sub, std::move(alignment)));
        }

        if (regID != -1)
        {
            ptr = std::make_unique<lir::PhysicalReg>(regID, mValue->getType()->getOperandSize());
            // See phi inst for why this is done
            if (dynamic_cast<AddrInst*>(mValue))
            {
                builder.addValue(std::make_unique<lir::LoadAddress>(std::move(ptr), std::move(value)));
            }
            else
            {
                builder.addValue(std::make_unique<lir::Move>(std::move(ptr), std::move(value)));
            }
        }
        else
        {
            int offset = 1 * 8;
            builder.addValue(std::make_unique<lir::Push>(std::move(value)));
        }
    }

    StoreParamInst::StoreParamInst(BasicBlock* parent, int paramIndex, Value* value, bool alignStack, const abi::CallingConvention* callingConvention)
            : Instruction(parent->getModule(), parent)
            , mCallingConvention(callingConvention)
            , mParamIndex(paramIndex)
            , mValue(value)
            , mAlignStack(alignStack)
    {
        mType = value->getType();
        mValue->setPreferredRegisterID(mCallingConvention->getParameterRegister(mParamIndex));
        mRequiresVReg = false;
    }
}