
#include "vipir/IR/Constant/ConstantStruct.h"
#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/GlobalVar.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/IR/Instruction/GEPInst.h"
#include "vipir/IR/Instruction/LoadInst.h"

#include "vipir/Type/StructType.h"
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
    auto structType = vipir::Type::GetStructType({i32Type, i64Type});

    auto func2 = vipir::Function::Create(vipir::FunctionType::Create(i32Type, {}), mod, "main");
    auto bb2 = vipir::BasicBlock::Create("", func2);

    builder.setInsertPoint(bb2);

    auto global = mod.createGlobalVar(structType);
    auto init1 = vipir::ConstantInt::Get(mod, 33, i32Type);
    auto init2 = vipir::ConstantInt::Get(mod, 69, i64Type);
    auto initVal = vipir::ConstantStruct::Get(mod, structType, {init1, init2});
    global->setInitialValue(initVal);

    auto ret = vipir::ConstantInt::Get(mod, 33, i32Type);

    builder.CreateRet(ret);

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}