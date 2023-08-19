#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"
#include "vipir/IR/Builder.h"

#include "vipir/Module.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/CallInst.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod ("test.tst");
    auto builder = vipir::Builder();

    vipir::Type* i32 = vipir::Type::GetIntegerType(32);

    auto fn2 = vipir::Function::Create(vipir::FunctionType::Get(i32), mod, "test");
    auto bb2 = vipir::BasicBlock::Create("", fn2);
    builder.setInsertPoint(bb2);
    auto retVal2 = builder.CreateConstantInt(33, vipir::Type::GetIntegerType(32));
    builder.CreateRet(retVal2);

    auto fn = vipir::Function::Create(vipir::FunctionType::Get(i32), mod, "main");
    auto bb = vipir::BasicBlock::Create("", fn);
    builder.setInsertPoint(bb);


    auto alloca = builder.CreateAlloca(vipir::Type::GetIntegerType(32));
    auto val = builder.CreateConstantInt(20, vipir::Type::GetIntegerType(32));
    builder.CreateStore(alloca, val);

    auto call = builder.CreateCall(fn2);

    //auto retVal = builder.CreateLoad(alloca);

    builder.CreateRet(call);

    std::ofstream f("file.out");
    mod.print(std::cout);

    mod.optimize(vipir::OptimizationLevel::High);
    
    std::cout << "\n";

    mod.emit(f);
}