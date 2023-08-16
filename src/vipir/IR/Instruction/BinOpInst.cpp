// Copyright 2023 solar-mist


#include "vipir/IR/Instruction/BinOpInst.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"

#include <format>

namespace vipir
{
    void BinOpInst::print(std::ostream& stream) const
    {
        std::string operatorName;
        switch (mOperator)
        {
            case Instruction::ADD:
                operatorName = "add";
                break;
            case Instruction::SUB:
                operatorName = "sub";
                break;
        }
        stream << std::format("%{} = {} {}, {}", mName, operatorName, mParent->getParent()->getValue(mLeft)->ident(), mParent->getParent()->getValue(mRight)->ident());
    }

    std::string BinOpInst::ident() const
    {
        return std::format("%{}", mName);
    }

    bool BinOpInst::requiresRegister() const
    {
        return false;
    }

    std::vector<ValueId> BinOpInst::getOperands()
    {
        return {mLeft, mRight};
    }

    void BinOpInst::emit(std::vector<instruction::ValuePtr>& values)
    {
        instruction::OperandPtr left   = mParent->getEmittedValue(mLeft);
        instruction::OperandPtr right = mParent->getEmittedValue(mRight);

        switch (mOperator)
        {
            case Instruction::ADD:
            {
                values.push_back(std::make_unique<instruction::AddInstruction>(left->clone(), std::move(right), codegen::OperandSize::None));
                break;
            }
            case Instruction::SUB:
            {
                values.push_back(std::make_unique<instruction::SubInstruction>(left->clone(), std::move(right), codegen::OperandSize::None));
                break;
            }
        }
        mEmittedValue = std::move(left);
    }

    BinOpInst::BinOpInst(BasicBlock* parent, ValueId id, Value* left, Instruction::BinaryOperators op, Value* right, std::string name)
        : Instruction(parent->getParent()->getModule(), parent, id)
        , mLeft(left->getID())
        , mOperator(op)
        , mRight(right->getID())
        , mName(name)
    {
    }
}