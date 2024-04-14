// Copyright 2024 solar-mist

#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/IR/Instruction/RetInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/BinaryInst.h"

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

    AllocaInst* IRBuilder::CreateAlloca(Type* allocatedType, std::string_view name)
    {
        AllocaInst* alloca = new AllocaInst(mInsertPoint, allocatedType, name);

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

    BinaryInst* IRBuilder::CreateAdd(Value* left, Value* right)
    {
        BinaryInst* add = new BinaryInst(mInsertPoint, left, Instruction::ADD, right);

        mInsertPoint->insertValue(add);

        return add;
    }

    BinaryInst* IRBuilder::CreateSub(Value* left, Value* right)
    {
        BinaryInst* add = new BinaryInst(mInsertPoint, left, Instruction::SUB, right);

        mInsertPoint->insertValue(add);

        return add;
    }

    BinaryInst* IRBuilder::CreateCmpEQ(Value* left, Value* right)
    {
        BinaryInst* add = new BinaryInst(mInsertPoint, left, Instruction::EQ, right);

        mInsertPoint->insertValue(add);

        return add;
    }

    BinaryInst* IRBuilder::CreateCmpNE(Value* left, Value* right)
    {
        BinaryInst* add = new BinaryInst(mInsertPoint, left, Instruction::NE, right);

        mInsertPoint->insertValue(add);

        return add;
    }


    ConstantInt* IRBuilder::CreateConstantInt(intmax_t value, Type* type)
    {
        ConstantInt* constantInt = new ConstantInt(mInsertPoint, value, type);

        mInsertPoint->insertValue(constantInt);

        return constantInt;
    }
}