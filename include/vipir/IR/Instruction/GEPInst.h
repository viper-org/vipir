// Copyright 2024 solar-mist

/*
 * An instruction that does arithmetic on
 * a pointer, such as adding or subtracting
*/

#ifndef VIPIR_IR_INSTRUCTION_GEP_INST_H
#define VIPIR_IR_INSTRUCTION_GEP_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class GEPInst : public Instruction
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;
    
        std::vector<Value*> getOperands() override;

    protected:
        void emit(MC::Builder& builder) override;

    private:
        GEPInst(BasicBlock* parent, Value* ptr, Value* offset);

        Value* mPtr;
        Value* mOffset;
        int mAlignment;

        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_GEP_INST_H