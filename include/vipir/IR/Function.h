// Copyright 2024 solar-mist

/*
 * Function IR nodes represent a function
 * definition or an extern function declaration
*/

#ifndef VIPIR_IR_FUNCTION_H
#define VIPIR_IR_FUNCTION_H 1

#include "vipir/IR/Global.h"
#include "vipir/IR/BasicBlock.h"

#include <memory>

namespace vipir
{
    class AllocaInst;
    class Function : public Global
    {
    friend class IRBuilder;
    using BasicBlockPtr = std::unique_ptr<BasicBlock>;
    public:
        static Function* Create(/*FunctionType* type, */Module& module, std::string_view name);

        void insertBasicBlock(BasicBlock* basicBlock);

        void print(std::ostream& stream) override;
        std::string ident() const override;

    protected:
        void emit(MC::Builder& builder) override;

    private:
        Function(/*FunctionType* type, */Module& module, std::string_view name);

        std::string mName;

        std::vector<BasicBlockPtr> mBasicBlockList;
        std::vector<AllocaInst*> mAllocaList;

        int mTotalStackOffset;
        void insertAlloca(AllocaInst* alloca);
        void setLocalStackOffsets();
    };
}

#endif // VIPIR_IR_FUNCTION_H