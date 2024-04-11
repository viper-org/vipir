// Copyright 2024 solar-mist

#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Instruction/RetInst.h"
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


    ConstantInt* IRBuilder::CreateConstantInt(intmax_t value)
    {
        ConstantInt* constantInt = new ConstantInt(mInsertPoint, value);

        mInsertPoint->insertValue(constantInt);

        return constantInt;
    }
}