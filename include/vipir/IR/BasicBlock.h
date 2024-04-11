// Copyright 2024 solar-mist

/*
 * Basic Blocks are a label that store
 * instructions and can be branched to,
 * conditionally or unconditionally
*/

#ifndef VIPIR_IR_BASIC_BLOCK_H
#define VIPIR_IR_BASIC_BLOCK_H 1

#include "vipir/IR/Value.h"

#include "vipir/IR/Instruction/Instruction.h"

#include <vector>

namespace vipir
{
    class Function;
    
    class BasicBlock : public Value
    {
    using InstructionPtr = std::unique_ptr<Instruction>;
    friend class Function;
    public:
        static BasicBlock* Create(std::string_view name, Function* parent);

        void insertInstruction(Instruction* instruction);

        void print(std::ostream& stream) override;

    protected:
        void emit(MC::Builder& builder) override;

    private:
        BasicBlock(std::string_view name, Function* parent);

        std::string mName;
        Function* mParent;

        std::vector<InstructionPtr> mInstructionList;
    };
}

#endif // VIPIR_IR_FUNCTION_H