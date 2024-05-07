// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/BinaryInst.h"

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/MC/CmpOperand.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Arithmetic.h"
#include "vipir/LIR/Instruction/Compare.h"
#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/instruction/variableOperandInstruction/IMulInstruction.h"

#include "vasm/instruction/operand/Immediate.h"

#include <cassert>
#include <format>

namespace vipir
{
    void BinaryInst::print(std::ostream& stream)
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

            case Instruction::IMUL:
                operatorName = "imul";
                break;

            case Instruction::BWOR:
                operatorName = "bwor";
                break;
            case Instruction::BWAND:
                operatorName = "bwand";
                break;
            case Instruction::BWXOR:
                operatorName = "bwxor";
                break;
            
            case Instruction::EQ:
                operatorName = "cmp eq";
                break;
            case Instruction::NE:
                operatorName = "cmp ne";
                break;
            
            case Instruction::LT:
                operatorName = "cmp lt";
                break;
            case Instruction::GT:
                operatorName = "cmp gt";
                break;
            
            case Instruction::LE:
                operatorName = "cmp le";
                break;
            case Instruction::GE:
                operatorName = "cmp ge";
                break;
        }
        stream << std::format("{} %{}, {}, {}", operatorName, getName(mValueId), mLeft->ident(), mRight->ident());
    }

    std::string BinaryInst::ident() const
    {
        return std::format("%{}", getName(mValueId));
    }

    std::vector<Value*> BinaryInst::getOperands()
    {
        return {mLeft, mRight};
    }

    void BinaryInst::emit(lir::Builder& builder)
    {

        auto createArithmetic = [&builder, this](lir::BinaryArithmetic::Operator op){
            mLeft->lateEmit(builder);
            mRight->lateEmit(builder);
            lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
            builder.addValue(std::make_unique<lir::Move>(vreg->clone(), mLeft->getEmittedValue()));
            builder.addValue(std::make_unique<lir::BinaryArithmetic>(vreg->clone(), op, mRight->getEmittedValue()));
            mEmittedValue = std::move(vreg);
        };
        
        switch (mOperator)
        {
            case Instruction::ADD:
                createArithmetic(lir::BinaryArithmetic::Operator::Add);
                break;
            case Instruction::SUB:
                createArithmetic(lir::BinaryArithmetic::Operator::Sub);
                break;

            case Instruction::IMUL:
                createArithmetic(lir::BinaryArithmetic::Operator::IMul);
                break;

            case Instruction::BWAND:
                createArithmetic(lir::BinaryArithmetic::Operator::BWAnd);
                break;
            case Instruction::BWOR:
                createArithmetic(lir::BinaryArithmetic::Operator::BWOr);
                break;
            case Instruction::BWXOR:
                createArithmetic(lir::BinaryArithmetic::Operator::BWXor);
                break;

            default:
                break;
        }
    }

    void BinaryInst::lateEmit(lir::Builder& builder)
    {
        auto createCompare = [&builder, this](lir::CMP::Operator op){
            mLeft->lateEmit(builder);
            mRight->lateEmit(builder);
            builder.addValue(std::make_unique<lir::Compare>(mLeft->getEmittedValue(), op, mRight->getEmittedValue()));
            mEmittedValue = std::make_unique<lir::CMP>(op);
        };

        switch (mOperator)
        {
            case Instruction::EQ:
                createCompare(lir::CMP::Operator::EQ);
                break;
            case Instruction::NE:
                createCompare(lir::CMP::Operator::NE);
                break;
            case Instruction::LT:
                createCompare(lir::CMP::Operator::LT);
                break;
            case Instruction::GT:
                createCompare(lir::CMP::Operator::GT);
                break;
            case Instruction::LE:
                createCompare(lir::CMP::Operator::LE);
                break;
            case Instruction::GE:
                createCompare(lir::CMP::Operator::GE);
                break;

            default:
                break;
        }
    }

    BinaryInst::BinaryInst(BasicBlock* parent, Value* left, Instruction::BinaryOperators op, Value* right)
        : Instruction(parent->getParent()->getModule(), parent)
        , mLeft(left)
        , mOperator(op)
        , mRight(right)
        , mValueId(mModule.getNextValueId())
    {
        assert(left->getType() == right->getType());

        mRequiresVReg = false;
        
        switch (mOperator)
        {
            case Instruction::ADD:
            case Instruction::SUB:
            case Instruction::IMUL:
            case Instruction::BWOR:
            case Instruction::BWAND:
            case Instruction::BWXOR:
            {
                mType = left->getType();
                mRequiresVReg = true;
                break;
            }

            case Instruction::EQ:
            case Instruction::NE:
            case Instruction::LT:
            case Instruction::GT:
            case Instruction::LE:
            case Instruction::GE:
            {
                mType = Type::GetBooleanType();
                break;
            }
        }
    }
}