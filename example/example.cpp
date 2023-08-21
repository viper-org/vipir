#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"
#include "vipir/IR/Builder.h"

#include "vipir/Module.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/AddrOfInst.h"
#include "vipir/IR/Instruction/BinOpInst.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod ("test.tst");
    auto builder = vipir::Builder();

    vipir::Type* i32 = vipir::Type::GetIntegerType(32);

    auto fn2 = vipir::Function::Create(vipir::FunctionType::Get(i32, {i32}), mod, "test");
    auto bb2 = vipir::BasicBlock::Create("", fn2);
    builder.setInsertPoint(bb2);
    auto retVal2 = fn2->getArgument(0);
    builder.CreateRet(retVal2);

    auto fn = vipir::Function::Create(vipir::FunctionType::Get(i32, {}), mod, "main");
    auto bb = vipir::BasicBlock::Create("", fn);
    builder.setInsertPoint(bb);


    auto alloca = builder.CreateAlloca(vipir::Type::GetIntegerType(32));
    auto val = builder.CreateConstantInt(20, vipir::Type::GetIntegerType(32));
    builder.CreateStore(alloca, val);

    auto load = builder.CreateAddrOf(alloca);
    auto constant = builder.CreateConstantInt(20, i32);
    auto cmp = builder.CreateICmpEQ(load, constant);

    auto bb3 = vipir::BasicBlock::Create("", fn);
    auto bb4 = vipir::BasicBlock::Create("", fn);
    builder.CreateCondBr(cmp, bb3, bb4);
    
    builder.setInsertPoint(bb3);
    builder.CreateRet(nullptr);

    builder.setInsertPoint(bb4);
    builder.CreateRet(nullptr);

    std::ofstream f("file.out");
    mod.print(std::cout);

    mod.optimize(vipir::OptimizationLevel::High);
    
    std::cout << "\n";

    mod.emit(f);
}