// Copyright 2023 solar-mist

/*
 * An instruction that executes a binary operation
 * such as adding or subtracting
*/

#ifndef VIPIR_IR_INSTRUCTION_SEXT_INST_H
#define VIPIR_IR_INSTRUCTION_SEXT_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;

    class SExtInst : public Instruction
    {
    friend class Builder;
    public:
        void print(std::ostream& stream) const override;
        std::string ident() const override;
        
        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) override;

    private:
        SExtInst(BasicBlock* parent, ValueId id, Value* source, Type* dest, std::string name);

        ValueId mSource;
        Type* mDest;
        std::string mName;
    };
}

#endif // VIPIR_IR_INSTRUCTION_SEXT_INST_H