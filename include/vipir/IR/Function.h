// Copyright 2024 solar-mist

/*
 * Function IR nodes represent a function
 * definition or an extern function declaration
*/

#ifndef VIPIR_IR_FUNCTION_H
#define VIPIR_IR_FUNCTION_H 1

#include "vipir/IR/Global.h"
#include "vipir/IR/Argument.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/Type/FunctionType.h"

#include <memory>

namespace vipir
{
    class AllocaInst;
    class Function : public Global
    {
    friend class Module;
    friend class IRBuilder;
    friend class opt::RegAlloc;
    friend class opt::DeadCodeEliminator;
    using BasicBlockPtr = std::unique_ptr<BasicBlock>;
    public:
        static Function* Create(FunctionType* type, Module& module, std::string_view name);

        FunctionType* getFunctionType() const;
        Argument* getArgument(int index) const;
        bool usesStack() const;
        std::vector<int> getCalleeSaved();

        void insertBasicBlock(BasicBlock* basicBlock);
        void setEmittedValue();

        void print(std::ostream& stream) override;
        std::string ident() const override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        Function(FunctionType* type, Module& module, std::string_view name);

        std::string mName;
        std::vector<ArgumentPtr> mArguments;

        std::vector<BasicBlockPtr> mBasicBlockList;

        std::vector<std::unique_ptr<opt::VReg> > mVirtualRegs;
        int mTotalStackOffset;
        std::vector<int> mCalleeSaved;

        lir::Value* mEnterFuncNode;
        std::vector<lir::Value*> mRetNodes;
        bool mHasCallNodes;

        void setCalleeSaved();
    };
}

#endif // VIPIR_IR_FUNCTION_H