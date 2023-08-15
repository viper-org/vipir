// Copyright 2023 solar-mist

/*
 * Function declaration or definition
*/

#ifndef VIPIR_IR_FUNCTION_H
#define VIPIR_IR_FUNCTION_H 1

#include "vipir/IR/Global.h"

#include <memory>
#include <string>
#include <deque>
#include <vector>

namespace vipir
{
    class BasicBlock;
    using BasicBlockPtr = std::unique_ptr<BasicBlock>;
    
    class Function : public Global
    {
    public:
        static Function* Create(Module& module, std::string name);

        Module& getModule() const;
        int& getInstructionCount();
        std::string_view getName() const;
        const std::vector<BasicBlockPtr>& getBasicBlockList() const;
        Value* getValue(ValueId index) const;
        int getNumValues() const;

        void insertBasicBlock(BasicBlock* basicBlock);
        void addValue(Value* value);

        void print(std::ostream& stream) const override;
        std::string ident() const override;

        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) override;

    private:
        Function(Module& module, std::string name);

        void sortAllocas();
        void allocateRegisters();

        std::string mName;
        std::vector<BasicBlockPtr> mBasicBlockList;
        std::vector<std::unique_ptr<Value>> mValues;
        int mInstructionCount;

        int mTotalStackOffset;
        std::deque<std::array<std::string_view, 4>> mRegisters;
    };
}

#endif // VIPIR_IR_FUNCTION_H