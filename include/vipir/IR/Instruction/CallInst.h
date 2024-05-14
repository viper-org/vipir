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
        
        std::vector<int> getRegisterSmashes() override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        CallInst(BasicBlock* parent, Function* function, std::vector<Value*> parameters);

        Function* mFunction;
        std::vector<Value*> mParameters;
        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_CALL_INST_H