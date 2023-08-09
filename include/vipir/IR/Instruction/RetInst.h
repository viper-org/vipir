// Copyright 2023 solar-mist

/*
 * An instruction that returns from a function to its
 * caller
*/

#ifndef VIPIR_IR_INSTRUCTION_RET_INST_H
#define VIPIR_IR_INSTRUCTION_RET_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;

    class RetInst : public Instruction
    {
    friend class Builder;
    public:
        void print(std::ostream& stream) const override;
        std::string ident() const override;

    protected:
        void emit(std::ostream& stream) const final;

    private:
        RetInst(BasicBlock* parent);
    };
}

#endif // VIPIR_IR_INSTRUCTION_RET_INST_H