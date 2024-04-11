// Copyright 2024 solar-mist

#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vasm/instruction/Label.h"

#include <format>

namespace vipir
{
    Function* Function::Create(/*FunctionType* type, */Module& module, std::string_view name)
    {
        Function* func = new Function(/*type, */module, name);

        module.insertGlobal(func);

        return func;
    }

    void Function::print(std::ostream& stream)
    {
        stream << std::format("\n\nfunction @{}() -> i32 {{", mName);
        stream << "\n\n";
        stream << "}";
    }

    void Function::emit(MC::Builder& builder)
    {
        builder.addValue(std::make_unique<instruction::Label>(mName));
    }

    Function::Function(/*FunctionType* type, */Module& module, std::string_view name)
        : Global(module)
        , mName(name)
        /*,mType(type)*/
    {
    }
}