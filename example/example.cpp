
#include "vipir/IR/IRBuilder.h"
#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/LoadInst.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod("test.tst");
    vipir::IRBuilder builder;

    auto func = vipir::Function::Create(mod, "main");
    auto bb1 = vipir::BasicBlock::Create("test", func);
    auto bb2 = vipir::BasicBlock::Create("test2", func);

    builder.setInsertPoint(bb1);

    auto local1 = builder.CreateAlloca("testvar1");
    auto local1Value = builder.CreateConstantInt(123);

    builder.CreateStore(local1, local1Value);
    auto retval = builder.CreateLoad(local1);
    builder.CreateRet(retval);

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}