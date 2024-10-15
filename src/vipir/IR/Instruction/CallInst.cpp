// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/CallInst.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vipir/Type/PointerType.h"

#include "vipir/LIR/Operand.h"
#include "vipir/LIR/Instruction/Move.h"
#include "vipir/LIR/Instruction/Jump.h"
#include "vipir/LIR/Instruction/Arithmetic.h"

#include "vasm/instruction/operand/Register.h"

#include "vasm/instruction/singleOperandInstruction/CallInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    void CallInst::print(std::ostream& stream)
    {
        stream << std::format("call {}(", mCallee->ident());

        if (!mParameters.empty())
        {
            for (int i = 0; i < mParameters.size() - 1; ++i)
            {
                stream << mParameters[i]->ident() << ", ";
            }
            stream << mParameters.back()->ident();
        }

        stream << std::format("), %{}", getName(mValueId));
    }

    std::string CallInst::ident() const
    {
        return std::format("%{}", getName(mValueId));
    }

    std::vector<int> CallInst::getRegisterSmashes()
    {
        return mModule.abi()->getCallerSavedRegisters();
    }


    void CallInst::emit(lir::Builder& builder)
    {
        mCallee->lateEmit(builder);

        lir::OperandPtr callee = mCallee->getEmittedValue();
        codegen::OperandSize size = mType->getOperandSize();

        builder.addValue(std::make_unique<lir::Call>(std::move(callee)));

        if (mStackRestore > 0)
        {
            assert(mStackRestore % 16 == 0);

            lir::OperandPtr stack = std::make_unique<lir::PhysicalReg>(mModule.abi()->getStackArgumentRegister(), codegen::OperandSize::Quad);
            lir::OperandPtr restore = std::make_unique<lir::Immediate>(mStackRestore);
            builder.addValue(std::make_unique<lir::BinaryArithmetic>(std::move(stack), lir::BinaryArithmetic::Operator::Add, std::move(restore)));
        }

        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
        lir::OperandPtr returnRegister = std::make_unique<lir::PhysicalReg>(mModule.abi()->getReturnRegister(), mType->getOperandSize());
        builder.addValue(std::make_unique<lir::Move>(vreg->clone(), std::move(returnRegister)));
        mEmittedValue = std::move(vreg);
    }

    CallInst::CallInst(BasicBlock* parent, Value* callee, std::vector<Value*> parameters, int stackRestore)
        : Instruction(parent->getModule(), parent)
        , mCallee(callee)
        , mParameters(std::move(parameters))
        , mStackRestore(stackRestore)
        , mValueId(mModule.getNextValueId())
    {
        FunctionType* functionType;
        if (callee->getType()->isPointerType())
        {
            functionType = static_cast<FunctionType*>(static_cast<PointerType*>(mCallee->getType())->getBaseType());
        }
        else
            functionType = static_cast<FunctionType*>(mCallee->getType());
        mType = functionType->getReturnType();

        int index = 0;
        for (auto parameter : mParameters)
        {
            assert(parameter->getType() == functionType->getArgumentTypes()[index++]);
        }
    }
}