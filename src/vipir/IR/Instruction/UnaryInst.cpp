// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/UnaryInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Arithmetic.h"
#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/singleOperandInstruction/Grp4Instruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    void UnaryInst::print(std::ostream& stream)
    {
        std::string operatorName;
        switch (mOperator)
        {
            case Instruction::NEG:
                operatorName = "neg";
                break;
            case Instruction::NOT:
                operatorName = "not";
                break;
        }
        stream << std::format("{} %{}, {}", operatorName, getName(mValueId), mOperand->ident());
    }

    std::string UnaryInst::ident() const
    {
        return std::format("%{}", getName(mValueId));
    }

    void UnaryInst::doConstantFold()
    {
        if (mOperand->isConstantFolded())
        {
            std::uintmax_t value;
            switch (mOperator)
            {
                case Instruction::NEG:
                    value = -mOperand->getConstantFoldedValue();
                    break;
                case Instruction::NOT:
                    value = ~mOperand->getConstantFoldedValue();
                    break;
            }
            mConstantFoldedValue = value;
            mIsConstantFolded = true;
            mRequiresVReg = false; // No need for a virtual reg since emitted value will be an IMM
        }
    }

    std::vector<Value*> UnaryInst::getOperands()
    {
        return {mOperand};
    }


    void UnaryInst::emit(lir::Builder& builder)
    {
        auto createUnary = [&builder, this](lir::UnaryArithmetic::Operator op){
            if (mIsConstantFolded)
            {
                mEmittedValue = std::make_unique<lir::Immediate>(mConstantFoldedValue);
                return;
            }
            mOperand->lateEmit(builder);
            lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
            builder.addValue(std::make_unique<lir::Move>(vreg->clone(), mOperand->getEmittedValue()));
            builder.addValue(std::make_unique<lir::UnaryArithmetic>(vreg->clone(), op));
            mEmittedValue = std::move(vreg);
        };

        switch (mOperator)
        {
            case Instruction::NEG:
                createUnary(lir::UnaryArithmetic::Operator::Neg);
                break;
            case Instruction::NOT:
                createUnary(lir::UnaryArithmetic::Operator::Not);
                break;
        }
    }

    UnaryInst::UnaryInst(BasicBlock* parent, Value* operand, Instruction::UnaryOperators op)
        : Instruction(parent->getParent()->getModule(), parent)
        , mOperand(operand)
        , mOperator(op)
        , mValueId(mModule.getNextValueId())
    {
        switch (mOperator)
        {
            case Instruction::NEG:
                mType = mOperand->getType();
            case Instruction::NOT:
                mType = mOperand->getType();
        }
    }
}