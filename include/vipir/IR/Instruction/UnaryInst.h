// Copyright 2024 solar-mist

/*
 * An instruction that executes a unary operation
 * such as negation
*/

#ifndef VIPIR_IR_INSTRUCTION_UNARY_INST_H
#define VIPIR_IR_INSTRUCTION_UNARY_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class UnaryInst : public Instruction
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;
    
        std::vector<Value*> getOperands() override;

    protected:
        void emit(MC::Builder& builder) override;
        void emit2(lir::Builder& builder) override;

    private:
        UnaryInst(BasicBlock* parent, Value* operand, Instruction::UnaryOperators op);

        Value* mOperand;
        Instruction::UnaryOperators mOperator;

        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_UNARY_INST_H