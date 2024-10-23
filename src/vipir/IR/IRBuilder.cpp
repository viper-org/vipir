// Copyright 2024 solar-mist

#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/IR/Instruction/RetInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/IR/Instruction/GEPInst.h"
#include "vipir/IR/Instruction/BinaryInst.h"
#include "vipir/IR/Instruction/UnaryInst.h"
#include "vipir/IR/Instruction/BranchInst.h"
#include "vipir/IR/Instruction/CallInst.h"
#include "vipir/IR/Instruction/StoreParamInst.h"
#include "vipir/IR/Instruction/PtrCastInst.h"
#include "vipir/IR/Instruction/SExtInst.h"
#include "vipir/IR/Instruction/ZExtInst.h"
#include "vipir/IR/Instruction/TruncInst.h"
#include "vipir/IR/Instruction/IntToPtrInst.h"
#include "vipir/IR/Instruction/PtrToIntInst.h"
#include "vipir/IR/Instruction/PhiInst.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Constant/ConstantBool.h"

#include "vipir/Module.h"

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



    AllocaInst* IRBuilder::CreateAlloca(Type* allocatedType)
    {
        AllocaInst* alloca = new AllocaInst(mInsertPoint, allocatedType);

        mInsertPoint->insertValue(alloca);

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

    AddrInst* IRBuilder::CreateAddrOf(Value* ptr)
    {
        AddrInst* addr = new AddrInst(mInsertPoint, ptr);

        mInsertPoint->insertValue(addr);

        return addr;
    }

    GEPInst* IRBuilder::CreateGEP(Value* ptr, Value* offset)
    {
        GEPInst* gep = new GEPInst(mInsertPoint, ptr, offset);

        mInsertPoint->insertValue(gep);

        return gep;
    }
    GEPInst* IRBuilder::CreateStructGEP(Value* ptr, int index)
    {
        GEPInst* gep = new GEPInst(mInsertPoint, ptr, ConstantInt::Get(mInsertPoint->getModule(), index, Type::GetIntegerType(32)));

        mInsertPoint->insertValue(gep);

        return gep;
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

    BinaryInst* IRBuilder::CreateSMul(Value* left, Value* right)
    {
        BinaryInst* smul = new BinaryInst(mInsertPoint, left, Instruction::SMUL, right);

        mInsertPoint->insertValue(smul);

        return smul;
    }

    BinaryInst* IRBuilder::CreateUMul(Value* left, Value* right)
    {
        BinaryInst* umul = new BinaryInst(mInsertPoint, left, Instruction::UMUL, right);

        mInsertPoint->insertValue(umul);

        return umul;
    }

    BinaryInst* IRBuilder::CreateSDiv(Value* left, Value* right)
    {
        BinaryInst* sdiv = new BinaryInst(mInsertPoint, left, Instruction::SDIV, right);

        mInsertPoint->insertValue(sdiv);

        return sdiv;
    }

    BinaryInst* IRBuilder::CreateUDiv(Value* left, Value* right)
    {
        BinaryInst* udiv = new BinaryInst(mInsertPoint, left, Instruction::UDIV, right);

        mInsertPoint->insertValue(udiv);

        return udiv;
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


    CallInst* IRBuilder::CreateCall(Function* function, std::vector<Value*> parameters)
    {
        int index = 0;
        bool alignedStack = true;
        int stackRestore = 0;
        if (parameters.size() >= mInsertPoint->getModule().abi()->getParameterRegisterCount())
        {
            stackRestore = 8 * (parameters.size() - mInsertPoint->getModule().abi()->getParameterRegisterCount());
            stackRestore = (stackRestore + 15) & ~15; // align to 16 bytes
            if ((parameters.size() - mInsertPoint->getModule().abi()->getParameterRegisterCount()) % 2 != 0) // misaligned stack
                alignedStack = false;
        }

        std::vector<StoreParamInst*> stores;
        for (auto parameter : parameters)
        {
            StoreParamInst* store = new StoreParamInst(mInsertPoint, index++, parameter, !alignedStack);
            stores.push_back(store);
            if (!alignedStack)
                alignedStack = true;
            mInsertPoint->insertValue(store);
        }

        CallInst* call = new CallInst(mInsertPoint, function, std::move(parameters), stackRestore);

        for (auto store : stores)
        {
            store->mCall = call;
        }

        mInsertPoint->insertValue(call);

        return call;
    }

    PtrCastInst* IRBuilder::CreatePtrCast(Value* ptr, Type* destType)
    {
        PtrCastInst* cast = new PtrCastInst(mInsertPoint, ptr, destType);

        mInsertPoint->insertValue(cast);

        return cast;
    }

    SExtInst* IRBuilder::CreateSExt(Value* value, Type* destType)
    {
        SExtInst* sext = new SExtInst(mInsertPoint, value, destType);

        mInsertPoint->insertValue(sext);

        return sext;
    }

    ZExtInst* IRBuilder::CreateZExt(Value* value, Type* destType)
    {
        ZExtInst* zext = new ZExtInst(mInsertPoint, value, destType);

        mInsertPoint->insertValue(zext);

        return zext;
    }

    TruncInst* IRBuilder::CreateTrunc(Value* value, Type* destType)
    {
        TruncInst* trunc = new TruncInst(mInsertPoint, value, destType);

        mInsertPoint->insertValue(trunc);

        return trunc;
    }

    IntToPtrInst* IRBuilder::CreateIntToPtr(Value* value, Type* destType)
    {
        IntToPtrInst* inttoptr = new IntToPtrInst(mInsertPoint, value, destType);

        mInsertPoint->insertValue(inttoptr);

        return inttoptr;
    }

    PtrToIntInst* IRBuilder::CreatePtrToInt(Value* value, Type* destType)
    {
        PtrToIntInst* ptrtoint = new PtrToIntInst(mInsertPoint, value, destType);

        mInsertPoint->insertValue(ptrtoint);

        return ptrtoint;
    }


    PhiInst* IRBuilder::CreatePhi(Type* type)
    {
        PhiInst* phi = new PhiInst(mInsertPoint, type);

        mInsertPoint->insertValue(phi);

        return phi;
    }


    ConstantBool* IRBuilder::CreateConstantBool(bool value)
    {
        ConstantBool* constantBool = new ConstantBool(mInsertPoint, value);

        mInsertPoint->insertValue(constantBool);

        return constantBool;
    }
}