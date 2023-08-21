// Copyright 2023 solar-mist

/*
 * An instruction that executes a binary operation
 * such as adding or subtracting
*/

#ifndef VIPIR_IR_INSTRUCTION_BIN_OP_INST_H
#define VIPIR_IR_INSTRUCTION_BIN_OP_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;

    class BinOpInst : public Instruction
    {
    friend class Builder;
    public:
        void print(std::ostream& stream) const override;
        std::string ident() const override;
        
        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) override;

    private:
        BinOpInst(BasicBlock* parent, ValueId id, Value* left, Instruction::BinaryOperators op, Value* right, std::string name);

        instruction::OperandPtr getEmittedValue() override;

        ValueId mLeft;
        Instruction::BinaryOperators mOperator;
        ValueId mRight;
        std::string mName;

        std::vector<instruction::ValuePtr>* mValues; // For comparison operations that are emit-on-use
    };
}

#endif // VIPIR_IR_INSTRUCTION_BIN_OP_INST_H