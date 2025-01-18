// Copyright 2024 solar-mist

/*
 * An instruction that stores a value into a parameter
 * register by its index
*/

#ifndef VIPIR_IR_INSTRUCTION_STORE_PARAM_INST_H
#define VIPIR_IR_INSTRUCTION_STORE_PARAM_INST_H 1

#include "vipir/ABI/CallingConvention.h"

#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    class StoreParamInst : public Instruction
    {
        friend class IRBuilder;

    public:
        void print(std::ostream& stream) override;

        void doConstantFold() override;

        std::vector<std::reference_wrapper<Value*> > getOperands() override;
        std::vector<int> getRegisterSmashes() override;

        bool isCritical() override;

    protected:
        void emit(lir::Builder& builder) override;
        std::string ident() const override;

    private:
        StoreParamInst(BasicBlock* parent, int paramIndex, Value* value, bool alignStack, const abi::CallingConvention* callingConvention);

        const abi::CallingConvention* mCallingConvention;
        int mParamIndex;
        Value* mValue;
        bool mAlignStack;

        Value* mCall;
    };
}

#endif // VIPIR_IR_INSTRUCTION_STORE_PARAM_INST_H