// Copyright 2023 solar-mist


#include "vipir/IR/Function.h"
#include "vipir/Module.h"

#include <format>

namespace vipir
{
    Function* Function::Create(Module& module, std::string name)
    {
        Function* func = new Function(module, std::move(name));

        module.insertGlobal(func);

        return func;
    }

    Module& Function::getModule() const
    {
        return mModule;
    }
    
    std::string_view Function::getName() const
    {
        return mName;
    }

    void Function::print(std::ostream& stream) const
    {
        stream << std::format("define pub void @{}() {{\n}}", mName);
    }

    std::string Function::ident() const
    {
        return std::format("@{}", mName);
    }

    Function::Function(Module& module, std::string name)
        :Global(module), mName(std::move(name))
    {
    }
}