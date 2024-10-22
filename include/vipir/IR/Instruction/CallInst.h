// Copyright 2024 solar-mist

/*
 * An instruction that calls a function
*/

#ifndef VIPIR_IR_INSTRUCTION_CALL_INST_H
#define VIPIR_IR_INSTRUCTION_CALL_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class Function;

    class CallInst : public Instruction
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;
    
        std::vector<Value*> getOperands() override;
        
        std::vector<int> getRegisterSmashes() override;

        bool isCritical() override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        CallInst(BasicBlock* parent, Value* callee, std::vector<Value*> parameters, int stackRestore);

        Value* mCallee;
        std::vector<Value*> mParameters;
        int mStackRestore;
        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_CALL_INST_H