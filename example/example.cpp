
#include "vipir/IR/IRBuilder.h"
#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/Type/Type.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod("test.tst");
    vipir::IRBuilder builder;

    auto i32Type = vipir::Type::GetIntegerType(64);
    auto i64Type = vipir::Type::GetIntegerType(32);

    auto func = vipir::Function::Create(mod, "main");
    auto bb1 = vipir::BasicBlock::Create("test", func);
    auto bb2 = vipir::BasicBlock::Create("test2", func);

    builder.setInsertPoint(bb1);

    auto local1 = builder.CreateAlloca(i32Type, "testvar1");
    auto local1Value = builder.CreateConstantInt(123, i32Type);

    builder.CreateStore(local1, local1Value);
    auto retval = builder.CreateLoad(local1);
    builder.CreateRet(retval);

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}