// Copyright 2024 solar-mist


#include "vipir/MC/StructOperand.h"

namespace vipir
{
    StructOperand::StructOperand(std::vector<instruction::OperandPtr> values)
        : mValues(std::move(values))
    {
    }

    std::vector<instruction::OperandPtr>& StructOperand::getValues()
    {
        return mValues;
    }

    std::unique_ptr<instruction::Operand> StructOperand::clone()
    {
        std::vector<instruction::OperandPtr> values;
        for (auto& value : mValues)
        {
            values.push_back(value->clone());
        }
        return std::make_unique<StructOperand>(std::move(values));
    }
}