// Copyright 2023 solar-mist

/*
 * An instruction that loads a value from a pointer
 * such as an AllocaInst
*/

#ifndef VIPIR_IR_INSTRUCTION_LOAD_INST_H
#define VIPIR_IR_INSTRUCTION_LOAD_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;

    class LoadInst : public Instruction
    {
    friend class Builder;
    public:
        void print(std::ostream& stream) const override;
        std::string ident() const override;
        
        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

        Value* getPointer() const;

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) override;

    private:
        LoadInst(BasicBlock* parent, ValueId id, Value* ptr, std::string name);

        ValueId mPtr;
        std::string mName;
    };
}

#endif // VIPIR_IR_INSTRUCTION_LOAD_INST_H