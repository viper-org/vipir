#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"
#include "vipir/Module.h"

#include <iostream>

int main()
{
    vipir::Module mod ("test.tst");

    auto fn = vipir::Function::Create(mod, "test");

    auto bb = vipir::BasicBlock::Create("", fn);

    mod.print(std::cout);
    
    std::cout << "\n";

    mod.emit(std::cout);
}