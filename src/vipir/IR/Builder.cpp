// Copyright 2023 solar-mist


#include "vipir/IR/Builder.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/IR/Instruction/RetInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/BinOpInst.h"

#include "vipir/IR/Constant/ConstantInt.h"

namespace vipir
{
    Builder::Builder()
    {
    }

    void Builder::setInsertPoint(BasicBlock* newInsertPoint)
    {
        mInsertPoint = newInsertPoint;
    }

    RetInst* Builder::CreateRet(Value* returnValue)
    {
        ValueId id = mInsertPoint->getParent()->getNumValues();
        RetInst* ret = new RetInst(mInsertPoint, id, returnValue);

        mInsertPoint->insertValue(ret);
        mInsertPoint->getParent()->addValue(ret);

        return ret;
    }

    AllocaInst* Builder::CreateAlloca(Type* allocatedType, std::string name)
    {
        ValueId id = mInsertPoint->getParent()->getNumValues();
        if (name.empty())
        {
            name = std::to_string(id);
        }

        AllocaInst* alloca = new AllocaInst(mInsertPoint, id, allocatedType, name);

        mInsertPoint->insertValue(alloca);
        mInsertPoint->getParent()->addValue(alloca);

        return alloca;
    }

    StoreInst* Builder::CreateStore(Value* ptr, Value* value)
    {
        ValueId id = mInsertPoint->getParent()->getNumValues();
        StoreInst* store = new StoreInst(mInsertPoint, id, ptr, value);

        mInsertPoint->insertValue(store);
        mInsertPoint->getParent()->addValue(store);

        return store;
    }

    LoadInst* Builder::CreateLoad(Value* ptr, std::string name)
    {
        ValueId id = mInsertPoint->getParent()->getNumValues();
        if (name.empty())
        {
            name = std::to_string(id);
        }

        LoadInst* load = new LoadInst(mInsertPoint, id, ptr, name);

        mInsertPoint->insertValue(load);
        mInsertPoint->getParent()->addValue(load);

        return load;
    }
    
    BinOpInst* Builder::CreateAdd(Value* left, Value* right, std::string name)
    {
        ValueId id = mInsertPoint->getParent()->getNumValues();
        if (name.empty())
        {
            name = std::to_string(id);
        }

        BinOpInst* binOp = new BinOpInst(mInsertPoint, id, left, Instruction::ADD, right, name);

        mInsertPoint->insertValue(binOp);
        mInsertPoint->getParent()->addValue(binOp);

        return binOp;
    }

    BinOpInst* Builder::CreateSub(Value* left, Value* right, std::string name)
    {
        ValueId id = mInsertPoint->getParent()->getNumValues();
        if (name.empty())
        {
            name = std::to_string(id);
        }

        BinOpInst* binOp = new BinOpInst(mInsertPoint, id, left, Instruction::SUB, right, name);

        mInsertPoint->insertValue(binOp);
        mInsertPoint->getParent()->addValue(binOp);

        return binOp;
    }

    ConstantInt* Builder::CreateConstantInt(uint64_t value, Type* type, std::string name)
    {
        ValueId id = mInsertPoint->getParent()->getNumValues();
        if (name.empty())
        {
            name = std::to_string(id);
        }

        ConstantInt* constant = new ConstantInt(mInsertPoint, id, value, type, name);

        mInsertPoint->insertValue(constant);
        mInsertPoint->getParent()->addValue(constant);

        return constant;
    }
}