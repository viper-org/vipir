// Copyright 2023 solar-mist

/*
 * An instruction that branches to another
 * basic block, conditionally or unconditionally
 */

#ifndef VIPIR_IR_INSTRUCTION_BRANCH_INST_H
#define VIPIR_IR_INSTRUCTION_BRANCH_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class BranchInst : public Instruction
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;
        
        std::vector<Value*> getOperands() override;

        bool isCritical() override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        BranchInst(BasicBlock* parent, BasicBlock* destination); // Unconditional branch
        BranchInst(BasicBlock* parent, Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch); // Conditional branch

        Value* mCondition;
        BasicBlock* mTrueBranch;
        BasicBlock* mFalseBranch;
    };
}

#endif // VIPIR_IR_INSTRUCTION_BRANCH_INST_H