// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/StoreParamInst.h"
#include "vipir/IR/Constant/ConstantStruct.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/AddrInst.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

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

        if (mType->isStructType())
        {
            auto constant = dynamic_cast<ConstantStruct*>(mValue);
            if (constant)
            {
                auto compound = static_cast<lir::Compound*>(value.get());
                for (auto it = compound->getValues().rbegin(); it != compound->getValues().rend(); ++it)
                {
                    builder.addValue(std::make_unique<lir::Push>((*it)->clone()));
                }
                return;
            }

            auto ptr = mValue->getEmittedValue();
            if (dynamic_cast<AllocaInst*>(mValue))
            {
                ptr = std::make_unique<lir::Memory>(mType->getOperandSize(), std::move(ptr), std::nullopt, nullptr, std::nullopt);
            }
            else
            {
                ptr = std::make_unique<lir::Memory>(mType->getOperandSize(), std::move(ptr), std::nullopt, nullptr, std::nullopt);
            }
            
            // Push in reverse order so the struct is laid out correctly for the callee
            for (int i = mType->getSizeInBits() / 8 - 8; i >= 0; i -= 8)
            {
                auto mem = static_cast<lir::Memory*>(ptr.get());
                auto ptr = mem->clone();
                mem = static_cast<lir::Memory*>(ptr.get());
                mem->addDisplacement(i);
                builder.addValue(std::make_unique<lir::Push>(std::move(ptr)));
                // TODO(IMPORTANT): Make this use mov instead of push
            }
            return;
        }

        if (regID != -1)
        {
            ptr = std::make_unique<lir::PhysicalReg>(regID, mValue->getType()->getOperandSize());
            builder.addValue(std::make_unique<lir::Move>(std::move(ptr), std::move(value)));
        }
        else
        {
            int offset = (mParamIndex - 1) * 8;
            if (mParent->getParent()->getCallingConvention()->reserveRegisterParameterStack())
            {
				offset += mCallingConvention->getParameterRegisterCount() * 8;
            }
            builder.addValue(std::make_unique<lir::Move>(std::make_unique<lir::StackSlot>(offset), std::move(value)));
        }
    }

    StoreParamInst::StoreParamInst(BasicBlock* parent, int paramIndex, Value* value, const abi::CallingConvention* callingConvention)
            : Instruction(parent->getModule(), parent)
            , mCallingConvention(callingConvention)
            , mParamIndex(paramIndex)
            , mValue(value)
    {
        if (mValue->getType()->isStructType())
        {
            if (auto load = dynamic_cast<LoadInst*>(mValue))
            {
                auto ptr = load->getPointer();
                mValue = ptr;
            }
        }

        mType = value->getType();
        mValue->setPreferredRegisterID(mCallingConvention->getParameterRegister(mParamIndex));
        mRequiresVReg = false;
    }
}