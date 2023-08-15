// Copyright 2023 solar-mist


#include "vipir/IR/Builder.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/IR/Instruction/RetInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/StoreInst.h"

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
        RetInst* ret = new RetInst(mInsertPoint, returnValue);

        mInsertPoint->insertInstruction(ret);

        return ret;
    }

    AllocaInst* Builder::CreateAlloca(Type* allocatedType, std::string name)
    {
        if (name.empty())
        {
            name = std::to_string(mInsertPoint->getParent()->getInstructionCount()++);
        }

        AllocaInst* alloca = new AllocaInst(mInsertPoint, allocatedType, name);

        mInsertPoint->insertInstruction(alloca);

        return alloca;
    }

    StoreInst* Builder::CreateStore(Value* ptr, Value* value)
    {
        StoreInst* store = new StoreInst(mInsertPoint, ptr, value);

        mInsertPoint->insertInstruction(store);

        return store;
    }

    ConstantInt* Builder::CreateConstantInt(uint64_t value, Type* type, std::string name)
    {
        if (name.empty())
        {
            name = std::to_string(mInsertPoint->getParent()->getInstructionCount()++);
        }

        return new ConstantInt(mInsertPoint, value, type, name);
    }
}