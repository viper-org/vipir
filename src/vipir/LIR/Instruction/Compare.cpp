// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Compare.h"

#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        Compare::Compare(OperandPtr left, CMP::Operator op, OperandPtr right)
            : mLeft(std::move(left))
            , mOperator(op)
            , mRight(std::move(right))
        {
        }

        void Compare::print(std::ostream& stream) const
        {
            std::string operatorName;
            std::string symbol;
            switch (mOperator)
            {
                case CMP::Operator::EQ:
                    operatorName = "EQ";
                    symbol = "=";
                    break;
                case CMP::Operator::NE:
                    operatorName = "NE";
                    symbol = "!=";
                    break;

                case CMP::Operator::LT:
                    operatorName = "LT";
                    symbol = "<";
                    break;
                case CMP::Operator::GT:
                    operatorName = "GT";
                    symbol = ">";
                    break;

                case CMP::Operator::LE:
                    operatorName = "LE";
                    symbol = "<=";
                    break;
                case CMP::Operator::GE:
                    operatorName = "GE";
                    symbol = ">=";
                    break;
            }
            stream << std::format("{} {} {} {} -> %CMP\n", operatorName, mLeft->ident(), symbol, mRight->ident());
        }

        void Compare::emit(MC::Builder& builder)
        {
            if (dynamic_cast<VirtualReg*>(mRight.get()) || dynamic_cast<PhysicalReg*>(mRight.get()))
            {
                builder.addValue(std::make_unique<instruction::CmpInstruction>(mRight->asmOperand(), mLeft->asmOperand()));
            }
            else
            {
                builder.addValue(std::make_unique<instruction::CmpInstruction>(mLeft->asmOperand(), mRight->asmOperand()));
            }
        }

        std::vector<std::reference_wrapper<OperandPtr> > Compare::getInputOperands()
        {
            return {mLeft, mRight};
        }
    }
}