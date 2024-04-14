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
}