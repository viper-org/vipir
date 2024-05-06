// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Arithmetic.h"

#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"
#include "vasm/instruction/singleOperandInstruction/Grp4Instruction.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        BinaryArithmetic::BinaryArithmetic(OperandPtr left, Operator op, OperandPtr right)
            : mLeft(std::move(left))
            , mOperator(op)
            , mRight(std::move(right))
        {
        }

        void BinaryArithmetic::print(std::ostream& stream) const
        {
            std::string operatorName;
            std::string symbol;
            switch (mOperator)
            {
                case Operator::Add:
                    operatorName = "ADD";
                    symbol = "+";
                    break;
                case Operator::Sub:
                    operatorName = "SUB";
                    symbol = "-";
                    break;

                case Operator::BWAnd:
                    operatorName = "AND";
                    symbol = "&";
                    break;
                case Operator::BWOr:
                    operatorName = "OR";
                    symbol = "|";
                    break;
                case Operator::BWXor:
                    operatorName = "XOR";
                    symbol = "^";
                    break;
            }
            stream << std::format("{} {} {} {} -> {}\n", operatorName, mLeft->ident(), symbol, mRight->ident(), mLeft->ident());
        }

        void BinaryArithmetic::emit(MC::Builder& builder)
        {
            auto createInstruction = [&builder, this]<class InstT>(){
                builder.addValue(std::make_unique<InstT>(mLeft->asmOperand(), mRight->asmOperand()));
            };
            
            switch(mOperator)
            {
                case Operator::Add:
                    createInstruction.template operator()<instruction::AddInstruction>();
                    break;
                case Operator::Sub:
                    createInstruction.template operator()<instruction::SubInstruction>();
                    break;

                case Operator::BWAnd:
                    createInstruction.template operator()<instruction::AndInstruction>();
                    break;
                case Operator::BWOr:
                    createInstruction.template operator()<instruction::OrInstruction>();
                    break;
                case Operator::BWXor:
                    createInstruction.template operator()<instruction::XorInstruction>();
                    break;
            }
        }


        UnaryArithmetic::UnaryArithmetic(OperandPtr operand, Operator op)
            : mOperand(std::move(operand))
            , mOperator(op)
        {
        }

        void UnaryArithmetic::print(std::ostream& stream) const
        {
            std::string operatorName;
            switch (mOperator)
            {
                case Operator::Not:
                    operatorName = "NOT";
                    break;

                case Operator::Neg:
                    operatorName = "NEG";
                    break;
            }

            stream << std::format("{} {} -> {}", operatorName, mOperand->ident(), mOperand->ident());
        }

        void UnaryArithmetic::emit(MC::Builder& builder)
        {
            auto createInstruction = [&builder, this]<class InstT>(){
                builder.addValue(std::make_unique<InstT>(mOperand->asmOperand()));
            };

            switch (mOperator)
            {
                case Operator::Not:
                    createInstruction.template operator()<instruction::NotInstruction>();
                    break;

                case Operator::Neg:
                    createInstruction.template operator()<instruction::NegInstruction>();
                    break;
            }
        }
    }
}