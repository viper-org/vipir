// Copyright 2024 solar-mist

/*
 * An instruction that sets a value based on
 * its predecessor basicblock
*/

#ifndef VIPIR_IR_INSTRUCTION_PHI_INST_H
#define VIPIR_IR_INSTRUCTION_PHI_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class PhiInst : public Instruction
    {
    friend class IRBuilder;
    friend class BasicBlock;
    friend class Function;
    friend class opt::RegAlloc;
    friend class opt::Mem2Reg;
    public:
        void print(std::ostream& stream) override;

        void doConstantFold() override;

        std::vector<std::reference_wrapper<Value*> > getOperands() override;
        void eraseFromParent() override;

        void addIncoming(Value* value, BasicBlock* basicBlock);

    protected:
        void emit(lir::Builder& builder) override;
        void setEmittedValue();
        std::string ident() const override;

    private:
        PhiInst(BasicBlock* parent, Type* type, AllocaInst* alloca = nullptr);

        AllocaInst* mAlloca;
        std::vector<std::pair<Value*, BasicBlock*> > mIncoming;

        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_PHI_INST_H