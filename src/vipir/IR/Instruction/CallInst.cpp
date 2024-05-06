// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/CallInst.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/LIR/Operand.h"
#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"
#include "vipir/LIR/Instruction/Jump.h"

#include "vasm/instruction/operand/Register.h"

#include "vasm/instruction/singleOperandInstruction/CallInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    void CallInst::print(std::ostream& stream)
    {
        stream << std::format("call {}(", mFunction->ident());

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

    void CallInst::emit(MC::Builder& builder)
    {
        instruction::OperandPtr function = mFunction->getEmittedValue();
        codegen::OperandSize size = mType->getOperandSize();

        int index = 0;
        for (auto parameter : mParameters)
        {
            instruction::OperandPtr value = parameter->getEmittedValue();
            instruction::OperandPtr reg = std::make_unique<instruction::Register>(mModule.abi()->getParameterRegister(index++), parameter->getType()->getOperandSize());
            builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(reg), std::move(value)));
        }

        builder.addValue(std::make_unique<instruction::CallInstruction>(std::move(function)));
        
        instruction::OperandPtr operand = mVReg->operand(mType->getOperandSize());
        instruction::Register* reg = dynamic_cast<instruction::Register*>(operand.get());
        if (!reg || reg->getID() != mModule.abi()->getReturnRegister())
        {
            instruction::OperandPtr returnRegister = std::make_unique<instruction::Register>(mModule.abi()->getReturnRegister(), size);
            builder.addValue(std::make_unique<instruction::MovInstruction>(operand->clone(), std::move(returnRegister)));
        }
        mEmittedValue = std::move(operand);
    }

    void CallInst::emit2(lir::Builder& builder)
    {
        mFunction->lateEmit(builder);

        lir::OperandPtr function = mFunction->getEmittedValue2();
        codegen::OperandSize size = mType->getOperandSize();

        int index = 0;
        for (auto parameter : mParameters)
        {
            parameter->lateEmit(builder);
            lir::OperandPtr value = parameter->getEmittedValue2();
            lir::OperandPtr reg = std::make_unique<lir::PhysicalReg>(mModule.abi()->getParameterRegister(index++), parameter->getType()->getOperandSize());
            builder.addValue(std::make_unique<lir::Move>(std::move(reg), std::move(value)));
        }

        builder.addValue(std::make_unique<lir::Call>(std::move(function)));

        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
        lir::OperandPtr returnRegister = std::make_unique<lir::PhysicalReg>(mModule.abi()->getReturnRegister(), mType->getOperandSize());
        builder.addValue(std::make_unique<lir::Move>(vreg->clone(), std::move(returnRegister)));
        mEmittedValue2 = std::move(vreg);
    }

    CallInst::CallInst(BasicBlock* parent, Function* function, std::vector<Value*> parameters)
        : Instruction(parent->getModule(), parent)
        , mFunction(function)
        , mParameters(std::move(parameters))
        , mValueId(mModule.getNextValueId())
    {
        mType = mFunction->getFunctionType()->getReturnType();

        int index = 0;
        for (auto parameter : mParameters)
        {
            assert(parameter->getType() == mFunction->getArgument(index++)->getType());
        }
    }
}