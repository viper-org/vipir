#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"
#include "vipir/IR/Builder.h"
#include "vipir/Module.h"

#include <iostream>

int main()
{
    vipir::Module mod ("test.tst");

    auto fn = vipir::Function::Create(mod, "test");

    auto bb = vipir::BasicBlock::Create("", fn);

    auto builder = vipir::Builder();
    builder.setInsertPoint(bb);
    builder.CreateRet(nullptr);

    mod.print(std::cout);
    
    std::cout << "\n";

    mod.emit(std::cout);
}