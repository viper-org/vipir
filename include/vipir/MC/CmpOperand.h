// Copyright 2024 solar-mist

#ifndef VIPIR_MC_CMP_OPERAND_H
#define VIPIR_MC_CMP_OPERAND_H 1

#include "vasm/instruction/Operand.h"

namespace vipir
{
    enum class CmpOperator
    {
        EQ,
        NE,
    };

    class CmpOperand : public instruction::Operand
    {
    public:
        CmpOperand(CmpOperator op);

        CmpOperator getOperator() const;
        
        std::unique_ptr<instruction::Operand> clone() override;

    private:
        CmpOperator mOperator;
    };
}

#endif // VIPIR_MC_CMP_OPERAND_H