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
    friend class opt::RegAlloc;
    public:
        void print(std::ostream& stream) override;

        std::vector<Value*> getOperands() override;

        void addIncoming(Value* value, BasicBlock* basicBlock);

    protected:
        void emit(lir::Builder& builder) override;
        std::string ident() const override;

    private:
        PhiInst(BasicBlock* parent, Type* type);

        std::vector<std::pair<Value*, BasicBlock*> > mIncoming;

        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_PHI_INST_H