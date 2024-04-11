
#include "vipir/Module.h"

#include <iostream>
#include <fstream>

int main()
{
    vipir::Module mod("test.tst");

    mod.emit(std::cout, vipir::OutputFormat::ELF);
}