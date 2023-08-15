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
        
        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) override;

    private:
        RetInst(BasicBlock* parent, ValueId id, Value* returnValue);

        ValueId mReturnValue;
    };
}

#endif // VIPIR_IR_INSTRUCTION_RET_INST_H