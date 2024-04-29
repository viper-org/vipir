
#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/GlobalString.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Instruction/CallInst.h"
#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/IR/Instruction/GEPInst.h"
#include "vipir/IR/Instruction/LoadInst.h"

#include "vipir/Type/Type.h"

#include "vipir/ABI/SysV.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod("test.tst");
    mod.setABI<vipir::abi::SysV>();

    vipir::IRBuilder builder;

    auto i32Type = vipir::Type::GetIntegerType(32);
    auto i8Type = vipir::Type::GetIntegerType(8);

    // simple function that just returns its first argument
    auto func1 = vipir::Function::Create(vipir::FunctionType::Create(i32Type, {i32Type}), mod, "test");
    auto bb1 = vipir::BasicBlock::Create("", func1);
    builder.setInsertPoint(bb1);
    builder.CreateRet(func1->getArgument(0));


    // call the other function and return that
    auto func2 = vipir::Function::Create(vipir::FunctionType::Create(i8Type, {}), mod, "main");
    auto bb2 = vipir::BasicBlock::Create("", func2);

    builder.setInsertPoint(bb2);

    auto constant = builder.CreateConstantInt(1, i32Type);
    auto retval = builder.CreateCall(func1, {constant});

    auto str = vipir::GlobalString::Create(mod, "hi");
    auto strLoad = builder.CreateAddrOf(str);

    auto gep = builder.CreateGEP(strLoad, retval);
    auto ret = builder.CreateLoad(gep);

    builder.CreateRet(ret);

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}