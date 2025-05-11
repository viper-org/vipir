// Copyright 2025 solar-mist

/*
 * An instruction that selects between two values
 * based on a condition
 */

#ifndef VIPIR_IR_INSTRUCTION_SELECT_INST_H
#define VIPIR_IR_INSTRUCTION_SELECT_INST_H 1

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class SelectInst : public Instruction
    {
        friend class IRBuilder;

    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;

        std::vector<std::reference_wrapper<Value*> > getOperands() override;
        std::vector<int> getRegisterSmashes() override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        SelectInst(BasicBlock* parent, Value* condition, Value* trueValue, Value* falseValue);

        Value* mCondition;
        Value* mTrue;
        Value* mFalse;

        int mValueId;
    };
}

#endif // VIPIR_IR_INSTRUCTION_SELECT_INST_H