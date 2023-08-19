// Copyright 2023 solar-mist

/*
 * An instruction that calls another function, saving
 * the position in the current function
*/

#ifndef VIPIR_IR_INSTRUCTION_CALL_INST_H
#define VIPIR_IR_INSTRUCTION_CALL_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Builder;

    class CallInst : public Instruction
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
        CallInst(BasicBlock* parent, ValueId id, std::string name, Value* callee, const std::vector<Value*>& parameters);

        std::string mName;
        ValueId mCallee;
        std::vector<ValueId> mParameters;
    };
}

#endif // VIPIR_IR_INSTRUCTION_CALL_INST_H