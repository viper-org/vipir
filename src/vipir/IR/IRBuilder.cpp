// Copyright 2024 solar-mist

#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/IR/Debug/EmitDebugInfo.h"
#include "vipir/IR/Debug/QueryAddress.h"

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

    void IRBuilder::insertAfter(Value* value)
    {
        mInsertAfter = value;
    }


    RetInst* IRBuilder::CreateRet(Value* returnValue)
    {
        RetInst* ret = new RetInst(mInsertPoint, returnValue, mInsertPoint->getParent()->getCallingConvention()); // calling convention of current function should be all good

        mInsertPoint->insertValue(mInsertAfter, ret);

        return ret;
    }


    AllocaInst* IRBuilder::CreateAlloca(Type* allocatedType)
    {
        AllocaInst* alloca = new AllocaInst(mInsertPoint, allocatedType);

        mInsertPoint->insertValue(mInsertAfter, alloca);

        return alloca;
    }


    StoreInst* IRBuilder::CreateStore(Value* ptr, Value* value)
    {
        StoreInst* store = new StoreInst(mInsertPoint, ptr, value);

        mInsertPoint->insertValue(mInsertAfter, store);

        return store;
    }

    LoadInst* IRBuilder::CreateLoad(Value* ptr)
    {
        LoadInst* load = new LoadInst(mInsertPoint, ptr);

        mInsertPoint->insertValue(mInsertAfter, load);

        return load;
    }

    AddrInst* IRBuilder::CreateAddrOf(Value* ptr, DIVariable* debugVar)
    {
        AddrInst* addr = new AddrInst(mInsertPoint, ptr, debugVar);

        mInsertPoint->insertValue(mInsertAfter, addr);

        return addr;
    }

    GEPInst* IRBuilder::CreateGEP(Value* ptr, Value* offset)
    {
        GEPInst* gep = new GEPInst(mInsertPoint, ptr, offset);

        mInsertPoint->insertValue(mInsertAfter, gep);

        return gep;
    }

    GEPInst* IRBuilder::CreateStructGEP(Value* ptr, int index)
    {
        GEPInst* gep = new GEPInst(mInsertPoint, ptr, ConstantInt::Get(mInsertPoint->getModule(), index, Type::GetIntegerType(32)));

        mInsertPoint->insertValue(mInsertAfter, gep);

        return gep;
    }


    BinaryInst* IRBuilder::CreateAdd(Value* left, Value* right)
    {
        BinaryInst* add = new BinaryInst(mInsertPoint, left, Instruction::ADD, right);

        mInsertPoint->insertValue(mInsertAfter, add);

        return add;
    }

    BinaryInst* IRBuilder::CreateSub(Value* left, Value* right)
    {
        BinaryInst* sub = new BinaryInst(mInsertPoint, left, Instruction::SUB, right);

        mInsertPoint->insertValue(mInsertAfter, sub);

        return sub;
    }

    BinaryInst* IRBuilder::CreateSMul(Value* left, Value* right)
    {
        BinaryInst* smul = new BinaryInst(mInsertPoint, left, Instruction::SMUL, right);

        mInsertPoint->insertValue(mInsertAfter, smul);

        return smul;
    }

    BinaryInst* IRBuilder::CreateUMul(Value* left, Value* right)
    {
        BinaryInst* umul = new BinaryInst(mInsertPoint, left, Instruction::UMUL, right);

        mInsertPoint->insertValue(mInsertAfter, umul);

        return umul;
    }

    BinaryInst* IRBuilder::CreateSDiv(Value* left, Value* right)
    {
        BinaryInst* sdiv = new BinaryInst(mInsertPoint, left, Instruction::SDIV, right);

        mInsertPoint->insertValue(mInsertAfter, sdiv);

        return sdiv;
    }

    BinaryInst* IRBuilder::CreateUDiv(Value* left, Value* right)
    {
        BinaryInst* udiv = new BinaryInst(mInsertPoint, left, Instruction::UDIV, right);

        mInsertPoint->insertValue(mInsertAfter, udiv);

        return udiv;
    }

    BinaryInst* IRBuilder::CreateBWOr(Value* left, Value* right)
    {
        BinaryInst* bwor = new BinaryInst(mInsertPoint, left, Instruction::BWOR, right);

        mInsertPoint->insertValue(mInsertAfter, bwor);

        return bwor;
    }

    BinaryInst* IRBuilder::CreateBWAnd(Value* left, Value* right)
    {
        BinaryInst* bwand = new BinaryInst(mInsertPoint, left, Instruction::BWAND, right);

        mInsertPoint->insertValue(mInsertAfter, bwand);

        return bwand;
    }

    BinaryInst* IRBuilder::CreateBWXor(Value* left, Value* right)
    {
        BinaryInst* bwxor = new BinaryInst(mInsertPoint, left, Instruction::BWXOR, right);

        mInsertPoint->insertValue(mInsertAfter, bwxor);

        return bwxor;
    }

    BinaryInst* IRBuilder::CreateCmpEQ(Value* left, Value* right)
    {
        BinaryInst* eq = new BinaryInst(mInsertPoint, left, Instruction::EQ, right);

        mInsertPoint->insertValue(mInsertAfter, eq);

        return eq;
    }

    BinaryInst* IRBuilder::CreateCmpNE(Value* left, Value* right)
    {
        BinaryInst* ne = new BinaryInst(mInsertPoint, left, Instruction::NE, right);

        mInsertPoint->insertValue(mInsertAfter, ne);

        return ne;
    }

    BinaryInst* IRBuilder::CreateCmpLT(Value* left, Value* right)
    {
        BinaryInst* lt = new BinaryInst(mInsertPoint, left, Instruction::LT, right);

        mInsertPoint->insertValue(mInsertAfter, lt);

        return lt;
    }

    BinaryInst* IRBuilder::CreateCmpGT(Value* left, Value* right)
    {
        BinaryInst* gt = new BinaryInst(mInsertPoint, left, Instruction::GT, right);

        mInsertPoint->insertValue(mInsertAfter, gt);

        return gt;
    }

    BinaryInst* IRBuilder::CreateCmpLE(Value* left, Value* right)
    {
        BinaryInst* le = new BinaryInst(mInsertPoint, left, Instruction::LE, right);

        mInsertPoint->insertValue(mInsertAfter, le);

        return le;
    }

    BinaryInst* IRBuilder::CreateCmpGE(Value* left, Value* right)
    {
        BinaryInst* ge = new BinaryInst(mInsertPoint, left, Instruction::GE, right);

        mInsertPoint->insertValue(mInsertAfter, ge);

        return ge;
    }


    BranchInst* IRBuilder::CreateBr(BasicBlock* destination)
    {
        BranchInst* branch = new BranchInst(mInsertPoint, destination);

        mInsertPoint->insertValue(mInsertAfter, branch);

        return branch;
    }

    BranchInst* IRBuilder::CreateCondBr(Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch)
    {
        BranchInst* branch = new BranchInst(mInsertPoint, condition, trueBranch, falseBranch);

        mInsertPoint->insertValue(mInsertAfter, branch);

        return branch;
    }


    UnaryInst* IRBuilder::CreateNeg(Value* operand)
    {
        UnaryInst* neg = new UnaryInst(mInsertPoint, operand, Instruction::NEG);

        mInsertPoint->insertValue(mInsertAfter, neg);

        return neg;
    }

    UnaryInst* IRBuilder::CreateNot(Value* operand)
    {
        UnaryInst* notInst = new UnaryInst(mInsertPoint, operand, Instruction::NOT);

        mInsertPoint->insertValue(mInsertAfter, notInst);

        return notInst;
    }


    CallInst* IRBuilder::CreateCall(Function* function, std::vector<Value*> parameters)
    {
        int index = 0;
        bool alignedStack = true;
        int stackRestore = 0;

        if (function->getCallingConvention()->getStackCleaner() == abi::StackCleaner::Caller)
        {
            if (parameters.size() >= function->getCallingConvention()->getParameterRegisterCount())
            {
                stackRestore = 8 * (parameters.size() - function->getCallingConvention()->getParameterRegisterCount());
                stackRestore = (stackRestore + 15) & ~15; // align to 16 bytes
                if ((parameters.size() - function->getCallingConvention()->getParameterRegisterCount()) % 2 != 0) // misaligned stack
                    alignedStack = false;
            }
        }

        Value* insertAfter = mInsertAfter;
        std::vector<Value*> stores;

        for (int i = 0; i < function->getCallingConvention()->getParameterRegisterCount() && i < function->mArguments.size(); ++i)
        {
            StoreParamInst* store = new StoreParamInst(mInsertPoint, index++, parameters[i], !alignedStack, function->getCallingConvention());
            stores.push_back(store);
            if (!alignedStack)
                alignedStack = true;
            mInsertPoint->insertValue(insertAfter, store);
            insertAfter = store;
        }

        if (parameters.size() > function->getCallingConvention()->getParameterRegisterCount())
        {
            if (function->getCallingConvention()->getArgumentPassingOrder() == abi::ArgumentPassingOrder::RightToLeft)
            {
                for (auto it = parameters.rbegin(); it < parameters.rend() - function->getCallingConvention()->getParameterRegisterCount(); ++it)
                {
                    StoreParamInst* store = new StoreParamInst(mInsertPoint, index++, *it, !alignedStack, function->getCallingConvention());
                    stores.push_back(store);
                    if (!alignedStack)
                        alignedStack = true;
                    mInsertPoint->insertValue(insertAfter, store);
                    insertAfter = store;
                }
            }
            else if (function->getCallingConvention()->getArgumentPassingOrder() == abi::ArgumentPassingOrder::LeftToRight)
            {
                for (auto it = parameters.begin() + function->getCallingConvention()->getParameterRegisterCount(); it < parameters.end(); ++it)
                {
                    StoreParamInst* store = new StoreParamInst(mInsertPoint, index++, *it, !alignedStack, function->getCallingConvention());
                    stores.push_back(store);
                    if (!alignedStack)
                        alignedStack = true;
                    mInsertPoint->insertValue(insertAfter, store);
                    insertAfter = store;
                }
            }
        }

        CallInst* call = new CallInst(mInsertPoint, function, stores, stackRestore, function->getCallingConvention());

        for (auto store: stores)
        {
            static_cast<StoreParamInst*>(store)->mCall = call;
        }

        mInsertPoint->insertValue(insertAfter, call);

        return call;
    }

    PtrCastInst* IRBuilder::CreatePtrCast(Value* ptr, Type* destType)
    {
        PtrCastInst* cast = new PtrCastInst(mInsertPoint, ptr, destType);

        mInsertPoint->insertValue(mInsertAfter, cast);

        return cast;
    }

    SExtInst* IRBuilder::CreateSExt(Value* value, Type* destType)
    {
        SExtInst* sext = new SExtInst(mInsertPoint, value, destType);

        mInsertPoint->insertValue(mInsertAfter, sext);

        return sext;
    }

    ZExtInst* IRBuilder::CreateZExt(Value* value, Type* destType)
    {
        ZExtInst* zext = new ZExtInst(mInsertPoint, value, destType);

        mInsertPoint->insertValue(mInsertAfter, zext);

        return zext;
    }

    TruncInst* IRBuilder::CreateTrunc(Value* value, Type* destType)
    {
        TruncInst* trunc = new TruncInst(mInsertPoint, value, destType);

        mInsertPoint->insertValue(mInsertAfter, trunc);

        return trunc;
    }

    IntToPtrInst* IRBuilder::CreateIntToPtr(Value* value, Type* destType)
    {
        IntToPtrInst* inttoptr = new IntToPtrInst(mInsertPoint, value, destType);

        mInsertPoint->insertValue(mInsertAfter, inttoptr);

        return inttoptr;
    }

    PtrToIntInst* IRBuilder::CreatePtrToInt(Value* value, Type* destType)
    {
        PtrToIntInst* ptrtoint = new PtrToIntInst(mInsertPoint, value, destType);

        mInsertPoint->insertValue(mInsertAfter, ptrtoint);

        return ptrtoint;
    }


    PhiInst* IRBuilder::CreatePhi(Type* type)
    {
        PhiInst* phi = new PhiInst(mInsertPoint, type);

        mInsertPoint->insertValue(mInsertAfter, phi);

        return phi;
    }


    ConstantBool* IRBuilder::CreateConstantBool(bool value)
    {
        ConstantBool* constantBool = new ConstantBool(mInsertPoint, value);

        mInsertPoint->insertValue(mInsertAfter, constantBool);

        return constantBool;
    }


    EmitDebugInfo* IRBuilder::CreateDebugInfo(int line, int col)
    {
        EmitDebugInfo* info = new EmitDebugInfo(mInsertPoint->getModule(), line, col);

        mInsertPoint->insertValue(mInsertAfter, info);

        return info;
    }

    QueryAddress* IRBuilder::CreateQueryAddress()
    {
        QueryAddress* query = new QueryAddress(mInsertPoint->getModule());

        mInsertPoint->insertValue(mInsertAfter, query);

        return query;
    }
}