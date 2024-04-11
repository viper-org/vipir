// Copyright 2024 solar-mist

#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/IR/Instruction/RetInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/Instruction/LoadInst.h"

#include "vipir/IR/Constant/ConstantInt.h"

namespace vipir
{
    IRBuilder::IRBuilder()
    {
    }

    void IRBuilder::setInsertPoint(BasicBlock* newInsertPoint)
    {
        mInsertPoint = newInsertPoint;
    }

    BasicBlock* IRBuilder::getInsertPoint()
    {
        return mInsertPoint;
    }


    RetInst* IRBuilder::CreateRet(Value* returnValue)
    {
        RetInst* ret = new RetInst(mInsertPoint, returnValue);

        mInsertPoint->insertValue(ret);

        return ret;
    }

    AllocaInst* IRBuilder::CreateAlloca(std::string_view name)
    {
        AllocaInst* alloca = new AllocaInst(mInsertPoint, name);

        mInsertPoint->insertValue(alloca);
        mInsertPoint->getParent()->insertAlloca(alloca);

        return alloca;
    }

    StoreInst* IRBuilder::CreateStore(Value* ptr, Value* value)
    {
        StoreInst* store = new StoreInst(mInsertPoint, ptr, value);

        mInsertPoint->insertValue(store);

        return store;
    }

    LoadInst* IRBuilder::CreateLoad(Value* ptr)
    {
        LoadInst* load = new LoadInst(mInsertPoint, ptr);

        mInsertPoint->insertValue(load);

        return load;
    }


    ConstantInt* IRBuilder::CreateConstantInt(intmax_t value)
    {
        ConstantInt* constantInt = new ConstantInt(mInsertPoint, value);

        mInsertPoint->insertValue(constantInt);

        return constantInt;
    }
}