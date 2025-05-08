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
        LT,
        GT,
        LE,
        GE,
    };

    class CmpOperand : public instruction::Operand
    {
    public:
        CmpOperand(CmpOperator op);

        CmpOperator getOperator() const;
        
        std::unique_ptr<instruction::Operand> clone() override;

        std::string toString() override;

    private:
        CmpOperator mOperator;
    };
}

#endif // VIPIR_MC_CMP_OPERAND_H