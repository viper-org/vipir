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
    class PhiInst;

    class BasicBlock : public Value
    {
    using ValuePtr = std::unique_ptr<Value>;
    friend class Function;
    friend class PhiInst;
    friend class opt::RegAlloc;
    friend class opt::DeadCodeEliminator;
    friend class opt::AliasAnalyzer;
    friend class opt::DominatorAnalyzer;
    friend class opt::Mem2Reg;
    public:
        static BasicBlock* Create(std::string_view name, Function* parent);

        Function* getParent();

        void insertValue(Value* value);
        void insertValue(Value* insertAfter, Value* value);
        void eraseValue(Value* value);

        bool hasTerminator() const;

        std::vector<Value*>& liveIn();
        std::vector<BasicBlock*>& successors();
        std::vector<BasicBlock*>& predecessors();
        BasicBlock*& loopEnd();
        bool exists();
        lir::Value*& endNode();

        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;

        void cleanup() override;

    protected:
        void setEmittedValue();
        void emit(lir::Builder& builder) override;

        std::vector<PhiInst*>& getPhis();

    private:
        BasicBlock(std::string_view name, Function* parent);

        std::string mName;
        Function* mParent;

        std::vector<ValuePtr> mValueList;
        std::vector<Value*> mLiveIn;

        std::vector<BasicBlock*> mSuccessors;
        std::vector<BasicBlock*> mPredecessors;

        int mIndexInFunction;
        bool mDomCalculated { false };
        BasicBlock* mDom{ nullptr };
        std::vector<BasicBlock*> mDominanceFrontier;

        std::vector<PhiInst*> mPhis;
        BasicBlock* mLoopEnd;

        bool mExists;

        lir::Value* mEndNode{ nullptr }; // for phi nodes
    };
}

#endif // VIPIR_IR_FUNCTION_H