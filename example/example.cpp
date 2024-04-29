
#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/GlobalString.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Instruction/AllocaInst.h"
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
    auto voidType = vipir::Type::GetVoidType();
    auto structType = vipir::Type::GetStructType({i32Type, i32Type});

    // call the other function and return that
    auto func2 = vipir::Function::Create(vipir::FunctionType::Create(i32Type, {}), mod, "main");
    auto bb2 = vipir::BasicBlock::Create("", func2);

    builder.setInsertPoint(bb2);

    auto alloca = builder.CreateAlloca(structType, "test");
    auto gep = builder.CreateStructGEP(alloca, 1);
    auto constant = builder.CreateConstantInt(66, i32Type);
    builder.CreateStore(gep, constant);

    auto gep2 = builder.CreateStructGEP(alloca, 1);
    auto load = builder.CreateLoad(gep2);

    builder.CreateRet(load);

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}