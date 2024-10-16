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
#include "vipir/IR/Instruction/ZExtInst.h"

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
    auto boolType = vipir::Type::GetBooleanType();

    auto func1 = vipir::Function::Create(vipir::FunctionType::Create(boolType, {i32Type}), mod, "main");
    auto bb1 = vipir::BasicBlock::Create("", func1);

    builder.setInsertPoint(bb1);

    auto alloca = builder.CreateAlloca(i32Type);
    auto x = vipir::ConstantInt::Get(mod, 32, i32Type);
    builder.CreateStore(alloca, x);

    auto lhs = builder.CreateLoad(alloca);
    auto rhs = vipir::ConstantInt::Get(mod, 32, i32Type);
    auto cmp = builder.CreateCmpNE(lhs, rhs);
    
    auto deadAlloca = builder.CreateAlloca(i32Type);
    auto y = vipir::ConstantInt::Get(mod, 12, i32Type);
    builder.CreateStore(deadAlloca, y);

    builder.CreateRet(cmp);

    mod.addPass(vipir::Pass::DeadCodeElimination);

    //mod.print(std::cout);

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}