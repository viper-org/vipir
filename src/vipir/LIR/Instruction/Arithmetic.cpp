// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Arithmetic.h"

#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        Arithmetic::Arithmetic(OperandPtr left, Operator op, OperandPtr right)
            : mLeft(std::move(left))
            , mOperator(op)
            , mRight(std::move(right))
        {
        }

        void Arithmetic::print(std::ostream& stream) const
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
            }
            stream << std::format("{} {} {} {} -> {}\n", operatorName, mLeft->ident(), symbol, mRight->ident(), mLeft->ident());
        }

        void Arithmetic::emit(MC::Builder& builder)
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
            }
        }
    }
}