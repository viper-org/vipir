// Copyright 2023 solar-mist

#ifndef VIPIR_IR_INSTRUCTION_BRANCH_H
#define VIPIR_IR_INSTRUCTION_BRANCH_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;

    class BranchInst : public Instruction
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
        BranchInst(BasicBlock* parent, ValueId id, BasicBlock* destination); // Unconditional branch
        BranchInst(BasicBlock* parent, ValueId id, Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch); // Conditional branch

        ValueId mCondition;
        BasicBlock* mTrueBranch;
        BasicBlock* mFalseBranch;
    };
}

#endif