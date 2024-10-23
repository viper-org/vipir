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
    class Function;

    class RetInst;
    class AllocaInst;
    class StoreInst;
    class LoadInst;
    class AddrInst;
    class GEPInst;
    class BinaryInst;
    class UnaryInst;
    class BranchInst;
    class CallInst;
    class PtrCastInst;
    class SExtInst;
    class ZExtInst;
    class TruncInst;
    class IntToPtrInst;
    class PtrToIntInst;
    class PhiInst;

    class ConstantBool;

    class IRBuilder
    {
    public:
        IRBuilder();

        void setInsertPoint(BasicBlock* newInsertPoint);
        BasicBlock* getInsertPoint();

        void insertAfter(Value* value);


        RetInst* CreateRet(Value* returnValue);


        AllocaInst* CreateAlloca(Type* allocatedType);


        StoreInst* CreateStore(Value* ptr, Value* value);
        LoadInst* CreateLoad(Value* ptr);
        AddrInst* CreateAddrOf(Value* ptr);
        GEPInst* CreateGEP(Value* ptr, Value* offset);
        GEPInst* CreateStructGEP(Value* ptr, int index);


        BinaryInst* CreateAdd(Value* left, Value* right);
        BinaryInst* CreateSub(Value* left, Value* right);

        BinaryInst* CreateSMul(Value* left, Value* right);
        BinaryInst* CreateUMul(Value* left, Value* right);
        BinaryInst* CreateSDiv(Value* left, Value* right);
        BinaryInst* CreateUDiv(Value* left, Value* right);

        BinaryInst* CreateBWOr(Value* left, Value* right);
        BinaryInst* CreateBWAnd(Value* left, Value* right);
        BinaryInst* CreateBWXor(Value* left, Value* right);

        BinaryInst* CreateCmpEQ(Value* left, Value* right);
        BinaryInst* CreateCmpNE(Value* left, Value* right);
        BinaryInst* CreateCmpLT(Value* left, Value* right);
        BinaryInst* CreateCmpGT(Value* left, Value* right);
        BinaryInst* CreateCmpLE(Value* left, Value* right);
        BinaryInst* CreateCmpGE(Value* left, Value* right);


        UnaryInst* CreateNeg(Value* operand);
        UnaryInst* CreateNot(Value* operand);


        BranchInst* CreateBr(BasicBlock* destination);
        BranchInst* CreateCondBr(Value* condition, BasicBlock* trueBranch, BasicBlock* falseBranch);


        CallInst* CreateCall(Function* function, std::vector<Value*> parameters);


        PtrCastInst* CreatePtrCast(Value* ptr, Type* destType);
        SExtInst* CreateSExt(Value* value, Type* destType);
        ZExtInst* CreateZExt(Value* value, Type* destType);
        TruncInst* CreateTrunc(Value* value, Type* destType);
        IntToPtrInst* CreateIntToPtr(Value* value, Type* destType);
        PtrToIntInst* CreatePtrToInt(Value* value, Type* destType);

        PhiInst* CreatePhi(Type* type);


        ConstantBool* CreateConstantBool(bool value);

    private:
        BasicBlock* mInsertPoint;

        Value* mInsertAfter;
    };
}

#endif // VIPIR_IR_IRBUILDER_H