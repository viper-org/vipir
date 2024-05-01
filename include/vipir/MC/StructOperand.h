// Copyright 2024 solar-mist

#ifndef VIPIR_MC_STRUCT_OPERAND_H
#define VIPIR_MC_STRUCT_OPERAND_H 1

#include "vasm/instruction/Operand.h"

#include <vector>

namespace vipir
{
    class StructOperand : public instruction::Operand
    {
    public:
        StructOperand(std::vector<instruction::OperandPtr> values);

        std::vector<instruction::OperandPtr>& getValues();
        
        std::unique_ptr<instruction::Operand> clone() override;

    private:
        std::vector<instruction::OperandPtr> mValues;
    };
}

#endif // VIPIR_MC_STRUCT_OPERAND_H