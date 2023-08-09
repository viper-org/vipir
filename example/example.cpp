#include "vipir/IR/Function.h"
#include "vipir/Module.h"

#include <iostream>

int main()
{
    vipir::Module mod ("test.tst");

    vipir::Function::Create(mod, "test");

    mod.print(std::cout);
}