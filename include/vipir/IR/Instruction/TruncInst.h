// Copyright 2024 solar-mist

/*
 * An instruction that truncates an integral
 * value to a smaller integral type
*/

#ifndef VIPIR_IR_INSTRUCTION_TRUNC_INST_H
#define VIPIR_IR_INSTRUCTION_TRUNC_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class TruncInst : public Instruction
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;
    
        std::vector<Value*> getOperands() override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        TruncInst(BasicBlock* parent, Value* value, Type* destType);

        Value* mValue;

        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_TRUNC_INST_H