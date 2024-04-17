
#include "vipir/IR/IRBuilder.h"
#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/BinaryInst.h"
#include "vipir/IR/Instruction/UnaryInst.h"
#include "vipir/IR/Instruction/CallInst.h"

#include "vipir/Type/Type.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod("test.tst");
    vipir::IRBuilder builder;

    auto i32Type = vipir::Type::GetIntegerType(32);
    auto i64Type = vipir::Type::GetIntegerType(64);

    auto func2 = vipir::Function::Create(vipir::FunctionType::Create(i32Type), mod, "test4");
    auto bb3 = vipir::BasicBlock::Create("test3", func2);
    builder.setInsertPoint(bb3);
    auto val = builder.CreateConstantInt(69, i32Type);
    builder.CreateRet(val);

    auto func = vipir::Function::Create(vipir::FunctionType::Create(i32Type), mod, "main");
    auto bb1 = vipir::BasicBlock::Create("test", func);
    auto bb2 = vipir::BasicBlock::Create("test2", func);

    builder.setInsertPoint(bb1);

    auto local1 = builder.CreateAlloca(i32Type, "testvar1");
    auto local1Value = builder.CreateCall(func2);

    builder.CreateStore(local1, local1Value);

    auto test = builder.CreateLoad(local1);
    test->eraseFromParent();

    auto test2 = builder.CreateLoad(local1);

    auto retval = builder.CreateNeg(test2);
    builder.CreateRet(retval);

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}