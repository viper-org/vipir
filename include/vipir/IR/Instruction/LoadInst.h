// Copyright 2024 solar-mist

/*
 * An instruction that loads a value from an AllocaInst
 * or other pointer
*/

#ifndef VIPIR_IR_INSTRUCTION_LOAD_INST_H
#define VIPIR_IR_INSTRUCTION_LOAD_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;

    class LoadInst : public Instruction
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;

    protected:
        void emit(MC::Builder& builder) override;
        std::string ident() const override;

    private:
        LoadInst(BasicBlock* parent, Value* ptr);

        Value* mPtr;
        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_LOAD_INST_H