#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"
#include "vipir/IR/Builder.h"

#include "vipir/Module.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/BinOpInst.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod ("test.tst");

    auto fn = vipir::Function::Create(mod, "main");

    auto bb = vipir::BasicBlock::Create("", fn);

    auto builder = vipir::Builder();
    builder.setInsertPoint(bb);

    auto alloca = builder.CreateAlloca(vipir::Type::GetIntegerType(32));
    auto val = builder.CreateConstantInt(20, vipir::Type::GetIntegerType(32));
    builder.CreateStore(alloca, val);

    auto lhs = builder.CreateLoad(alloca);
    auto retVal = builder.CreateLoad(alloca);
    
    auto rhs = builder.CreateConstantInt(20, vipir::Type::GetIntegerType(32));
    builder.CreateICmpEQ(lhs, rhs);

    builder.CreateRet(retVal);

    std::ofstream f("file.out");
    mod.print(std::cout);
    
    std::cout << "\n";

    mod.emit(f);
}