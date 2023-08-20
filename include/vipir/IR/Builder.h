// Copyright 2023 solar-mist

/*
 * A builder is the object used to create IR objects
 * such as instructions
*/

#ifndef VIPIR_IR_BUILDER_H
#define VIPIR_IR_BUILDER_H 1

#include "vipir/IR/Value.h"

#include <cstdint>

namespace vipir
{
    class BasicBlock;

    class RetInst;
    class CallInst;
    class AllocaInst;
    class StoreInst;
    class LoadInst;
    class BinOpInst;
    class BranchInst;

    class ConstantInt;

    class Builder
    {
    public:
        Builder();

        void setInsertPoint(BasicBlock* newInsertPoint);

        RetInst* CreateRet(Value* returnValue);
        CallInst* CreateCall(Value* callee, std::vector<Value*> parameters, std::string name = "");

        AllocaInst* CreateAlloca(Type* allocatedType, std::string name = "");
        StoreInst* CreateStore(Value* ptr, Value* value);
        LoadInst* CreateLoad(Value* ptr, std::string name = "");

        BinOpInst* CreateAdd(Value* left, Value* right, std::string name = "");
        BinOpInst* CreateSub(Value* left, Value* right, std::string name = "");
        BinOpInst* CreateICmpEQ(Value* left, Value* right, std::string name = "");
        BinOpInst* CreateICmpNE(Value* left, Value* right, std::string name = "");

        BranchInst* CreateBr(BasicBlock* destination);


        ConstantInt* CreateConstantInt(uint64_t value, Type* type, std::string name = "");

    private:
        BasicBlock* mInsertPoint;
    };
}

#endif // VIPIR_IR_BUILDER_H