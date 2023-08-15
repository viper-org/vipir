// Copyright 2023 solar-mist

/*
 * An instruction that returns from a function to its
 * caller
*/

#ifndef VIPIR_IR_INSTRUCTION_RET_INST_H
#define VIPIR_IR_INSTRUCTION_RET_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;

    class RetInst : public Instruction
    {
    friend class Builder;
    public:
        Value* getReturnValue() const;
        Type* getReturnType() const;

        void print(std::ostream& stream) const override;
        std::string ident() const override;

    protected:
        instruction::OperandPtr emit(std::vector<instruction::ValuePtr>& values) override;

    private:
        RetInst(BasicBlock* parent, Value* returnValue);

        std::unique_ptr<Value> mReturnValue;
    };
}

#endif // VIPIR_IR_INSTRUCTION_RET_INST_H