// Copyright 2024 solar-mist

/*
 * Function IR nodes represent a function
 * definition or an extern function declaration
*/

#ifndef VIPIR_IR_FUNCTION_H
#define VIPIR_IR_FUNCTION_H 1

#include "vipir/ABI/CallingConvention.h"

#include "vipir/IR/Global.h"
#include "vipir/IR/Argument.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/DI/DIVariable.h"

#include "vipir/Type/FunctionType.h"

#include <memory>

namespace vipir
{
    class AllocaInst;

    class Function : public Global
    {
    friend class Module;
    friend class IRBuilder;
    friend class DIBuilder;
    friend class opt::RegAlloc;
    friend class opt::DeadCodeEliminator;
    friend class opt::AliasAnalyzer;
    friend class opt::DominatorAnalyzer;
    friend class opt::Mem2Reg;

    using BasicBlockPtr = std::unique_ptr<BasicBlock>;
    public:
        static Function* Create(FunctionType* type, Module& module, std::string_view name, bool pure, const abi::CallingConvention* callingConvention);
        static Function* Create(FunctionType* type, Module& module, std::string_view name, bool pure);
        
        void replaceAllUsesWith(Value* old, Value* newValue);

        const abi::CallingConvention* getCallingConvention() const;
        FunctionType* getFunctionType() const;
        Argument* getArgument(int index) const;
        bool usesStack() const;
        std::vector<int> getCalleeSaved();

        void insertBasicBlock(BasicBlock* basicBlock);
        int getNumBasicBlocks();
        std::vector<BasicBlockPtr>& getBasicBlocks();
        void setEmittedValue();
        bool isPure() const;

        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;

        void setDebugEndLocation(int endLine, int endCol);

    protected:
        void emit(lir::Builder& builder) override;

        void orderBasicBlocks();

        std::vector<AllocaInst*> getAllocaList();

    private:
        Function(FunctionType* type, Module& module, std::string_view name, bool pure,
                 const abi::CallingConvention* callingConvention);

        const abi::CallingConvention* mCallingConvention;

        std::string mName;
        std::vector<ArgumentPtr> mArguments;

        std::vector<BasicBlockPtr> mBasicBlockList;

        std::vector<std::unique_ptr<opt::VReg> > mVirtualRegs;
        int mTotalStackOffset;
        std::vector<int> mCalleeSaved;

        lir::Value* mEnterFuncNode;
        std::vector<lir::Value*> mRetNodes;
        bool mHasCallNodes;
        int mMaxCallParams{ 0 };

        bool mIsPure;

        std::vector<std::unique_ptr<DIVariable> > mDebugVariables;

        int mEndLine;
        int mEndCol;

        void setCalleeSaved();
    };
}

#endif // VIPIR_IR_FUNCTION_H