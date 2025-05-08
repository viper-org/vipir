// Copyright 2024 solar-mist

#include "vipir/MC/CmpOperand.h"

namespace vipir
{
    CmpOperand::CmpOperand(CmpOperator op)
        : mOperator(op)
    {
    }

    CmpOperator CmpOperand::getOperator() const
    {
        return mOperator;
    }

    std::unique_ptr<instruction::Operand> CmpOperand::clone()
    {
        return std::make_unique<CmpOperand>(mOperator);
    }

    std::string CmpOperand::toString()
    {
        return "Stray CMP operand found";
    }
}