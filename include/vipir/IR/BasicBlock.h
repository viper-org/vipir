// Copyright 2023 solar-mist

/*
 * BasicBlocks represent a label that stores
 * instructions and can be branched to (conditionally)
*/

#ifndef VIPIR_IR_BASIC_BLOCK_H
#define VIPIR_IR_BASIC_BLOCK_H 1

#include "vipir/IR/Value.h"

#include "vipir/IR/Instruction/Instruction.h"

#include <memory>
#include <vector>

namespace vipir
{
    class Function;

    using InstructionPtr = std::unique_ptr<Instruction>;
    
    class BasicBlock : public Value
    {
    friend class Function;
    public:
        static BasicBlock* Create(std::string name, Function* parent);

        Function* getParent() const;
        std::string_view getName() const;
        int getNoBranches() const;
        const std::vector<InstructionPtr>& getInstructionList() const;

        void insertInstruction(Instruction* instruction);

        void print(std::ostream& stream) const override;
        std::string ident() const override;

    protected:
        instruction::OperandPtr emit(std::vector<instruction::ValuePtr>& values) final;

    private:
        BasicBlock(std::string name, Function* parent);

        std::string mName;
        std::vector<InstructionPtr> mInstructionList;
        Function* mParent;
        int mBranches;
    };
}

#endif // VIPIR_IR_BASIC_BLOCK_H