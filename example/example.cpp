
#include "vipir/IR/IRBuilder.h"
#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/Type/Type.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod("test.tst");
    vipir::IRBuilder builder;

    auto i32Type = vipir::Type::GetIntegerType(32);

    auto func = vipir::Function::Create(vipir::FunctionType::Create(i32Type, {i32Type}), mod, "main");
    auto bb = vipir::BasicBlock::Create("test", func);

    builder.setInsertPoint(bb);

    builder.CreateRet(func->getArgument(0));

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}