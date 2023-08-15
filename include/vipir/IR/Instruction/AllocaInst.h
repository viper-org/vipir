// Copyright 2023 solar-mist

/*
 * An instruction that allocates a local variable
 * inside a function
*/

#ifndef VIPIR_IR_INSTRUCTION_ALLOCA_INST_H
#define VIPIR_IR_INSTRUCTION_ALLOCA_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;
    class Function;

    class AllocaInst : public Instruction
    {
    friend class Builder;
    friend class Function;
    public:
        Type* getAllocatedType() const;

        void print(std::ostream& stream) const override;
        std::string ident() const override;
        
        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) override;

    private:
        AllocaInst(BasicBlock* parent, ValueId id, Type* allocatedType, std::string name);

        std::string mName;
        Type* mAllocatedType;

        int mStackOffset;
    };
}

#endif // VIPIR_IR_INSTRUCTION_ALLOCA_INST_H