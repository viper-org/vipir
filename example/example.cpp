
#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/BasicBlock.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod("test.tst");

    auto func = vipir::Function::Create(mod, "main");
    auto bb1 = vipir::BasicBlock::Create("test", func);
    auto bb2 = vipir::BasicBlock::Create("test2", func);

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}