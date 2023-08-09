// Copyright 2023 solar-mist

/*
 * BasicBlocks represent a label that stores
 * instructions and can be branched to (conditionally)
*/

#ifndef VIPIR_IR_BASIC_BLOCK_H
#define VIPIR_IR_BASIC_BLOCK_H 1

#include "vipir/IR/Value.h"

#include <vector>

namespace vipir
{
    class Function;
    
    class BasicBlock : public Value
    {
    friend class Function;
    public:
        static BasicBlock* Create(std::string name, Function* parent);

        Function* getParent() const;
        std::string_view getName() const;
        int getNoBranches() const;

        void print(std::ostream& stream) const override;
        std::string ident() const override;

    protected:
        void emit(std::ostream& stream) const final;

    private:
        BasicBlock(std::string name, Function* parent);

        std::string mName;
        Function* mParent;
        int mBranches;
    };
}

#endif // MIDAS_IR_BASIC_BLOCK_H