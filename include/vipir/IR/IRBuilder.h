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

    class ConstantInt;

    class IRBuilder
    {
    public:
        IRBuilder();

        void setInsertPoint(BasicBlock* newInsertPoint);
        BasicBlock* getInsertPoint();

        RetInst* CreateRet(Value* returnValue);
        AllocaInst* CreateAlloca(std::string_view name);
        StoreInst* CreateStore(Value* ptr, Value* value);
        LoadInst* CreateLoad(Value* ptr);

        ConstantInt* CreateConstantInt(intmax_t value);

    private:
        BasicBlock* mInsertPoint;
    };
}

#endif // VIPIR_IR_IRBUILDER_H