// Copyright 2024 solar-mist

#include "vipir/Type/BooleanType.h"

namespace vipir
{
    BooleanType::BooleanType()
        : Type(8, "bool")
    {
        mOperandSize = codegen::OperandSize::Byte;
    }

    bool BooleanType::isBooleanType() const
    {
        return true;
    }
}