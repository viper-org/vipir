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
    class DIVariable;
    
    class AddrInst : public Instruction
    {
    friend class IRBuilder;
    friend class opt::AliasAnalyzer;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;
        
        std::vector<std::reference_wrapper<Value*> > getOperands() override;

        void doConstantFold() override;

        void cleanup() override;

        Value* getPointer();
        DIVariable* getDebugVariable();

    protected:
        void emit(lir::Builder& builder) override;

    private:
        AddrInst(BasicBlock* parent, Value* ptr, DIVariable* debugVar);

        Value* mPtr;
        int mValueId;

        DIVariable* mDebugVar;
    };
}

#endif // VIPIR_IR_INSTRUCTION_ADDR_INST_H