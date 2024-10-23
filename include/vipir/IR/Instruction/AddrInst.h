// Copyright 2024 solar-mist

/*
 * An instruction that gets the address of
 * an AllocaInst
*/

#ifndef VIPIR_IR_INSTRUCTION_ADDR_INST_H
#define VIPIR_IR_INSTRUCTION_ADDR_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class AddrInst : public Instruction
    {
    friend class IRBuilder;
    friend class opt::AliasAnalyzer;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;
        
        std::vector<Value*> getOperands() override;

        void doConstantFold() override;

        Value* getPointer();

    protected:
        void emit(lir::Builder& builder) override;

    private:
        AddrInst(BasicBlock* parent, Value* ptr);

        Value* mPtr;
        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_ADDR_INST_H