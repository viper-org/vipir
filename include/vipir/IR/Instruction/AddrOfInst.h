// Copyright 2023 solar-mist

/*
 * An instruction that gets the address of an AllocaInst
*/

#ifndef VIPIR_IR_INSTRUCTION_ADDR_OF_INST_H
#define VIPIR_IR_INSTRUCTION_ADDR_OF_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

#include "vipir/IR/Instruction/AllocaInst.h"

namespace vipir
{
    class Builder;

    class AddrOfInst : public Instruction
    {
    friend class Builder;
    public:
        void print(std::ostream& stream) const override;
        std::string ident() const override;
        
        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

        Value* getPointer() const;

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) override;

    private:
        AddrOfInst(BasicBlock* parent, ValueId id, AllocaInst* ptr, std::string name);

        ValueId mPtr;
        std::string mName;
    };
}

#endif // VIPIR_IR_INSTRUCTION_ADDR_OF_INST_H