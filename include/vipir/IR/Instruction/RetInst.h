// Copyright 2024 solar-mist

/*
 * An instruction that returns from a function to its
 * caller
*/

#ifndef VIPIR_IR_INSTRUCTION_RET_INST_H
#define VIPIR_IR_INSTRUCTION_RET_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class RetInst : public Instruction
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;

        std::vector<Value*> getOperands() override;

    protected:
        void emit(lir::Builder& builder) override;
        std::string ident() const override;

    private:
        RetInst(BasicBlock* parent, Value* returnValue);

        Value* mReturnValue;
    };
}

#endif // VIPIR_IR_INSTRUCTION_RET_INST_H