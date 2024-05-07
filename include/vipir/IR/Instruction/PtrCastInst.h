// Copyright 2024 solar-mist

/*
 * An instruction that casts a pointer
 * to a different pointer type
*/

#ifndef VIPIR_IR_INSTRUCTION_PTR_CAST_INST_H
#define VIPIR_IR_INSTRUCTION_PTR_CAST_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class PtrCastInst : public Instruction
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;
    
        std::vector<Value*> getOperands() override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        PtrCastInst(BasicBlock* parent, Value* ptr, Type* destType);

        Value* mPtr;

        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_PTR_CAST_INST_H