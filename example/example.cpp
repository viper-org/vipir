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
#include "vipir/IR/Instruction/PhiInst.h"

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

    auto func1 = vipir::Function::Create(vipir::FunctionType::Create(i32Type, {i32Type}), mod, "main");
    auto entrybb = vipir::BasicBlock::Create("", func1);
    auto bb1 = vipir::BasicBlock::Create("", func1);
    auto bb2 = vipir::BasicBlock::Create("", func1);
    auto mergebb = vipir::BasicBlock::Create("", func1);

    builder.setInsertPoint(entrybb);
    builder.CreateBr(bb1);


    builder.setInsertPoint(bb1);
    auto x1 = vipir::ConstantInt::Get(mod, 12, i32Type);
    builder.CreateBr(mergebb);

    builder.setInsertPoint(bb2);
    auto x2 = vipir::ConstantInt::Get(mod, 33, i32Type);
    builder.CreateBr(mergebb);

    
    builder.setInsertPoint(mergebb);
    auto phi = builder.CreatePhi(i32Type);
    phi->addIncoming(x1, bb1);
    phi->addIncoming(x2, bb2);
    builder.CreateRet(phi);

    mod.addPass(vipir::Pass::DeadCodeElimination);
    mod.addPass(vipir::Pass::PeepholeOptimization);

    //mod.print(std::cout);

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}