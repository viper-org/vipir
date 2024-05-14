#include "vipir/IR/GlobalString.h"
#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/GlobalVar.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Constant/ConstantStruct.h"
#include "vipir/IR/Constant/ConstantArray.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/IR/Instruction/CallInst.h"
#include "vipir/IR/Instruction/BinaryInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/GEPInst.h"
#include "vipir/IR/Instruction/TruncInst.h"
#include "vipir/IR/Instruction/SExtInst.h"

#include "vipir/Optimizer/RegAlloc/RegAlloc.h"
#include "vipir/Type/FunctionType.h"
#include "vipir/Type/StructType.h"
#include "vipir/Type/ArrayType.h"
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
    auto i64Type = vipir::Type::GetIntegerType(64);
    auto i8Type = vipir::Type::GetIntegerType(8);
    auto voidType = vipir::Type::GetVoidType();
    auto arrayType = vipir::Type::GetArrayType(i32Type, 3);
    auto structType = vipir::Type::GetStructType({arrayType, i32Type});

    auto func = vipir::Function::Create(vipir::FunctionType::Create(i32Type, {i32Type}), mod, "test");

    auto func1 = vipir::Function::Create(vipir::FunctionType::Create(i32Type, {i32Type}), mod, "main");
    auto bb1 = vipir::BasicBlock::Create("", func1);

    builder.setInsertPoint(bb1);

    auto alloca = builder.CreateAlloca(i32Type);
    auto x = vipir::ConstantInt::Get(mod, 32, i32Type);
    builder.CreateStore(alloca, x);

    auto param = builder.CreateLoad(alloca);

    auto call = builder.CreateCall(func, {param});

    builder.CreateRet(call);

    mod.addPass(vipir::Pass::PeepholeOptimization);

    //mod.print(std::cout);

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}