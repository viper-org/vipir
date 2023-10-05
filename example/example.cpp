#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"
#include "vipir/IR/Builder.h"

#include "vipir/Module.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/CallInst.h"
#include "vipir/IR/Instruction/SExtInst.h"

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
    
    auto alloca = builder.CreateAlloca(vipir::Type::GetIntegerType(16));
    auto val = builder.CreateConstantInt(33, vipir::Type::GetIntegerType(16));
    builder.CreateStore(alloca, val);

    auto load = builder.CreateLoad(alloca);
    auto retval = builder.CreateSExt(load, vipir::Type::GetIntegerType(32));
    builder.CreateRet(retval);

    auto fn = vipir::Function::Create(vipir::FunctionType::Get(i32, {}), mod, "main");
    auto bb = vipir::BasicBlock::Create("", fn);
    builder.setInsertPoint(bb);

    auto param = builder.CreateConstantInt(1, vipir::Type::GetIntegerType(32));
    auto call = builder.CreateCall(fn2, {param});
    builder.CreateRet(call);


    std::ofstream f("file.out");
    mod.print(std::cout);

    mod.optimize(vipir::OptimizationLevel::High);
    
    std::cout << "\n";

    mod.emit(f, vipir::OutputFormat::ELF);
}