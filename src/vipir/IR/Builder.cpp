// Copyright 2023 solar-mist


#include "vipir/IR/Builder.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Instruction/RetInst.h"

namespace vipir
{
    Builder::Builder()
    {
    }

    void Builder::setInsertPoint(BasicBlock* newInsertPoint)
    {
        mInsertPoint = newInsertPoint;
    }

    RetInst* Builder::CreateRet()
    {
        RetInst* ret = new RetInst(mInsertPoint);

        mInsertPoint->insertInstruction(ret);

        return ret;
    }
}