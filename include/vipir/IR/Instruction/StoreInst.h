// Copyright 2023 solar-mist

/*
 * An instruction that stores a value into an AllocaInst
 * or other pointer
*/

#ifndef VIPIR_IR_INSTRUCTION_STORE_INST_H
#define VIPIR_IR_INSTRUCTION_STORE_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;

    class StoreInst : public Instruction
    {
    friend class Builder;
    public:
        void print(std::ostream& stream) const override;
        std::string ident() const override;

    protected:
        instruction::OperandPtr emit(std::vector<instruction::ValuePtr>& values) override;

    private:
        StoreInst(BasicBlock* parent, Value* ptr, Value* value);

        Value* mPtr;
        std::unique_ptr<Value> mValue;
    };
}

#endif // VIPIR_IR_INSTRUCTION_STORE_INST_H