// Copyright 2023 solar-mist

/*
 * BasicBlocks represent a label that stores
 * instructions and can be branched to (conditionally)
*/

#ifndef VIPIR_IR_BASIC_BLOCK_H
#define VIPIR_IR_BASIC_BLOCK_H 1

#include "vipir/IR/Value.h"

#include <memory>
#include <vector>

namespace vipir
{
    class Function;
    enum class OptimizationLevel;
    
    class BasicBlock : public Value
    {
    friend class Function;
    public:
        static BasicBlock* Create(std::string name, Function* parent);

        Function* getParent() const;
        std::string_view getName() const;
        int getNoBranches() const;
        std::vector<ValueId>& getInstructionList();

        instruction::OperandPtr getEmittedValue(ValueId id);

        void insertValue(Value* value);
        void eraseValue(ValueId value);

        void print(std::ostream& stream) const override;
        std::string ident() const override;
        
        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

        void optimize(OptimizationLevel level);

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) final;

    private:
        BasicBlock(std::string name, Function* parent);

        std::string mName;
        std::vector<ValueId> mValueList;
        Function* mParent;
        int mBranches;
    };
}

#endif // VIPIR_IR_BASIC_BLOCK_H