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
    class AllocaInst : public Instruction
    {
    friend class IRBuilder;
    friend class opt::RegAlloc;
    public:
        void print(std::ostream& stream) override;

        void doConstantFold() override;

        Type* getAllocatedType();

        void forceMemory();

    protected:
        void emit(lir::Builder& builder) override;
        std::string ident() const override;

    private:
        AllocaInst(BasicBlock* parent, Type* allocatedType);

        Type* mAllocatedType;
        int mValueId;

        bool mForceMemory;
    };
}

#endif // VIPIR_IR_INSTRUCTION_ALLOCA_INST_H