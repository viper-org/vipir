// Copyright 2024 solar-mist

/*
 * An IR builder is the object used to create
 * IR objects such as instructions
*/

#ifndef VIPIR_IR_IRBUILDER_H
#define VIPIR_IR_IRBUILDER_H 1

#include "vipir/IR/Value.h"

#include <cstdint>

namespace vipir
{
    class BasicBlock;

    class RetInst;
    class AllocaInst;
    class StoreInst;
    class LoadInst;
    class BinaryInst;
    class BranchInst;

    class ConstantInt;

    class IRBuilder
    {
    public:
        IRBuilder();

        void setInsertPoint(BasicBlock* newInsertPoint);
        BasicBlock* getInsertPoint();

        RetInst* CreateRet(Value* returnValue);

        AllocaInst* CreateAlloca(Type* allocatedType, std::string_view name);

        StoreInst* CreateStore(Value* ptr, Value* value);
        LoadInst* CreateLoad(Value* ptr);

        BinaryInst* CreateAdd(Value* left, Value* right);
        BinaryInst* CreateSub(Value* left, Value* right);
        BinaryInst* CreateCmpEQ(Value* left, Value* right);
        BinaryInst* CreateCmpNE(Value* left, Value* right);

        BranchInst* CreateBr(BasicBlock* destination);
        BranchInst* CreateCondBr(Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch);

        ConstantInt* CreateConstantInt(intmax_t value, Type* type);

    private:
        BasicBlock* mInsertPoint;
    };
}

#endif // VIPIR_IR_IRBUILDER_H