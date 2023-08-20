// Copyright 2023 solar-mist

/*
 * Function declaration or definition
*/

#ifndef VIPIR_IR_FUNCTION_H
#define VIPIR_IR_FUNCTION_H 1

#include "vipir/IR/Global.h"
#include "vipir/IR/Argument.h"

#include "vipir/Type/FunctionType.h"

#include <memory>
#include <string>
#include <vector>

namespace vipir
{
    class BasicBlock;
    using BasicBlockPtr = std::unique_ptr<BasicBlock>;
    
    class Function : public Global
    {
    friend class Module;
    public:
        static Function* Create(FunctionType* type, Module& module, std::string name);

        Module& getModule() const;
        int& getInstructionCount();
        Type* getReturnType();
        std::string_view getName() const;
        const std::vector<BasicBlockPtr>& getBasicBlockList() const;
        Value* getValue(ValueId index) const;
        int getNumValues() const;
        Argument* getArgument(int index);
        instruction::OperandPtr getBasicBlockEmittedValue(BasicBlock* basicBlock) const;

        void insertBasicBlock(BasicBlock* basicBlock);
        void addValue(Value* value);

        void print(std::ostream& stream) const override;
        std::string ident() const override;

        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

        void optimize(OptimizationLevel level) override;

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) override;

    private:
        Function(FunctionType* type, Module& module, std::string name);

        instruction::OperandPtr getEmittedValue() override;

        void sortAllocas();
        void allocateRegisters();

        std::string mName;
        std::vector<BasicBlockPtr> mBasicBlockList;
        std::vector<std::unique_ptr<Value>> mValues;
        std::vector<ValueId> mArguments;
        int mInstructionCount;

        int mTotalStackOffset;
    };
}

#endif // VIPIR_IR_FUNCTION_H