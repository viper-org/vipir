// Copyright 2024 solar-mist

/*
 * An instruction that stores a value into an AllocaInst
 * or other pointer
*/

#ifndef VIPIR_IR_INSTRUCTION_STORE_INST_H
#define VIPIR_IR_INSTRUCTION_STORE_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class StoreInst : public Instruction
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
    
        std::vector<Value*> getOperands() override;

    protected:
        void emit(MC::Builder& builder) override;
        void emit2(lir::Builder& builder) override;
        std::string ident() const override;

    private:
        StoreInst(BasicBlock* parent, Value* ptr, Value* value);

        Value* mPtr;
        Value* mValue;
    };
}

#endif // VIPIR_IR_INSTRUCTION_STORE_INST_H