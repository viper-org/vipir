// Copyright 2024 solar-mist

/*
 * Basic Blocks are a label that store
 * instructions and can be branched to,
 * conditionally or unconditionally
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
    using ValuePtr = std::unique_ptr<Value>;
    friend class Function;
    friend class opt::RegAlloc;
    public:
        static BasicBlock* Create(std::string_view name, Function* parent);

        Function* getParent();

        void insertValue(Value* value);
        void eraseValue(Value* value);

        std::vector<Value*>& liveIn();
        std::vector<BasicBlock*>& successors();
        BasicBlock*& loopEnd();

        void print(std::ostream& stream) override;
        std::string ident() const override;

    protected:
        void setEmittedValue();
        void emit(lir::Builder& builder) override;

    private:
        BasicBlock(std::string_view name, Function* parent);

        std::string mName;
        Function* mParent;

        std::vector<ValuePtr> mValueList;
        std::vector<Value*> mLiveIn;
        std::vector<BasicBlock*> mSuccessors;
        BasicBlock* mLoopEnd;
    };
}

#endif // VIPIR_IR_FUNCTION_H