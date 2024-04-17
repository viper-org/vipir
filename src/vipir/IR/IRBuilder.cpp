// Copyright 2024 solar-mist

#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/IR/Instruction/RetInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/BinaryInst.h"
#include "vipir/IR/Instruction/UnaryInst.h"
#include "vipir/IR/Instruction/BranchInst.h"
#include "vipir/IR/Instruction/CallInst.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Constant/ConstantBool.h"

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
        BinaryInst* sub = new BinaryInst(mInsertPoint, left, Instruction::SUB, right);

        mInsertPoint->insertValue(sub);

        return sub;
    }

    BinaryInst* IRBuilder::CreateBWOr(Value* left, Value* right)
    {
        BinaryInst* bwor = new BinaryInst(mInsertPoint, left, Instruction::BWOR, right);

        mInsertPoint->insertValue(bwor);

        return bwor;
    }

    BinaryInst* IRBuilder::CreateBWAnd(Value* left, Value* right)
    {
        BinaryInst* bwand = new BinaryInst(mInsertPoint, left, Instruction::BWAND, right);

        mInsertPoint->insertValue(bwand);

        return bwand;
    }

    BinaryInst* IRBuilder::CreateBWXor(Value* left, Value* right)
    {
        BinaryInst* bwxor = new BinaryInst(mInsertPoint, left, Instruction::BWXOR, right);

        mInsertPoint->insertValue(bwxor);

        return bwxor;
    }

    BinaryInst* IRBuilder::CreateCmpEQ(Value* left, Value* right)
    {
        BinaryInst* eq = new BinaryInst(mInsertPoint, left, Instruction::EQ, right);

        mInsertPoint->insertValue(eq);

        return eq;
    }

    BinaryInst* IRBuilder::CreateCmpNE(Value* left, Value* right)
    {
        BinaryInst* ne = new BinaryInst(mInsertPoint, left, Instruction::NE, right);

        mInsertPoint->insertValue(ne);

        return ne;
    }

    BinaryInst* IRBuilder::CreateCmpLT(Value* left, Value* right)
    {
        BinaryInst* lt = new BinaryInst(mInsertPoint, left, Instruction::LT, right);

        mInsertPoint->insertValue(lt);

        return lt;
    }

    BinaryInst* IRBuilder::CreateCmpGT(Value* left, Value* right)
    {
        BinaryInst* gt = new BinaryInst(mInsertPoint, left, Instruction::GT, right);

        mInsertPoint->insertValue(gt);

        return gt;
    }

    BinaryInst* IRBuilder::CreateCmpLE(Value* left, Value* right)
    {
        BinaryInst* le = new BinaryInst(mInsertPoint, left, Instruction::LE, right);

        mInsertPoint->insertValue(le);

        return le;
    }

    BinaryInst* IRBuilder::CreateCmpGE(Value* left, Value* right)
    {
        BinaryInst* ge = new BinaryInst(mInsertPoint, left, Instruction::GE, right);

        mInsertPoint->insertValue(ge);

        return ge;
    }



    BranchInst* IRBuilder::CreateBr(BasicBlock* destination)
    {
        BranchInst* branch = new BranchInst(mInsertPoint, destination);

        mInsertPoint->insertValue(branch);

        return branch;
    }

    BranchInst* IRBuilder::CreateCondBr(Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch)
    {
        BranchInst* branch = new BranchInst(mInsertPoint, condition, trueBranch, falseBranch);

        mInsertPoint->insertValue(branch);

        return branch;
    }


    UnaryInst* IRBuilder::CreateNeg(Value* operand)
    {
        UnaryInst* neg = new UnaryInst(mInsertPoint, operand, Instruction::NEG);

        mInsertPoint->insertValue(neg);

        return neg;
    }

    UnaryInst* IRBuilder::CreateNot(Value* operand)
    {
        UnaryInst* notInst = new UnaryInst(mInsertPoint, operand, Instruction::NOT);

        mInsertPoint->insertValue(notInst);

        return notInst;
    }


    CallInst* IRBuilder::CreateCall(Function* function)
    {
        CallInst* call = new CallInst(mInsertPoint, function);

        mInsertPoint->insertValue(call);

        return call;
    }



    ConstantInt* IRBuilder::CreateConstantInt(intmax_t value, Type* type)
    {
        ConstantInt* constantInt = new ConstantInt(mInsertPoint, value, type);

        mInsertPoint->insertValue(constantInt);

        return constantInt;
    }

    ConstantBool* IRBuilder::CreateConstantBool(bool value)
    {
        ConstantBool* constantBool = new ConstantBool(mInsertPoint, value);

        mInsertPoint->insertValue(constantBool);

        return constantBool;
    }
}