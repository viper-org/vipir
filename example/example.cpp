
#include "vipir/Module.h"

#include "vipir/IR/Function.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod("test.tst");

    auto func = vipir::Function::Create(mod, "main");

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}