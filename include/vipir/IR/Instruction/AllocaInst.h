// Copyright 2024 solar-mist

/*
 * An instruction that allocates a local variable
 * inside of a function
*/

#ifndef VIPIR_IR_INSTRUCTION_ALLOCA_INST_H
#define VIPIR_IR_INSTRUCTION_ALLOCA_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;

    class AllocaInst : public Instruction
    {
    friend class IRBuilder;
    friend class Function; // So stack offset can be set
    public:
        void print(std::ostream& stream) override;

    protected:
        void emit(MC::Builder& builder) override;
        std::string ident() const override;

    private:
        AllocaInst(BasicBlock* parent, std::string_view name);

        std::string mName;

        int mStackOffset;
    };
}

#endif // VIPIR_IR_INSTRUCTION_RET_INST_H