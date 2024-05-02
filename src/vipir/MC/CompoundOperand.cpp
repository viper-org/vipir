// Copyright 2024 solar-mist


#include "vipir/MC/CompoundOperand.h"

namespace vipir
{
    CompoundOperand::CompoundOperand(std::vector<instruction::OperandPtr> values)
        : mValues(std::move(values))
    {
    }

    std::vector<instruction::OperandPtr>& CompoundOperand::getValues()
    {
        return mValues;
    }

    std::unique_ptr<instruction::Operand> CompoundOperand::clone()
    {
        std::vector<instruction::OperandPtr> values;
        for (auto& value : mValues)
        {
            values.push_back(value->clone());
        }
        return std::make_unique<CompoundOperand>(std::move(values));
    }
}