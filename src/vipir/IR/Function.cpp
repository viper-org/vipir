// Copyright 2024 solar-mist

#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/operand/Register.h"

#include "vasm/instruction/singleOperandInstruction/PushInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    Function* Function::Create(/*FunctionType* type, */Module& module, std::string_view name)
    {
        Function* func = new Function(/*type, */module, name);

        module.insertGlobal(func);

        return func;
    }

    void Function::insertBasicBlock(BasicBlock* basicBlock)
    {
        mBasicBlockList.push_back(BasicBlockPtr(basicBlock));
    }

    void Function::print(std::ostream& stream)
    {
        stream << std::format("\n\nfunction @{}() -> i32 {{\n", mName);
        for (auto& basicBlock : mBasicBlockList)
        {
            basicBlock->print(stream);
        }
        stream << "}";
    }

    void Function::emit(MC::Builder& builder)
    {
        builder.addValue(std::make_unique<instruction::Label>(mName));

        builder.addValue(std::make_unique<instruction::PushInstruction>(instruction::Register::Get("rbp")));
        builder.addValue(std::make_unique<instruction::MovInstruction>(instruction::Register::Get("rbp"), instruction::Register::Get("rsp"), codegen::OperandSize::None));

        for (auto& basicBlock : mBasicBlockList)
        {
            basicBlock->emit(builder);
        }
    }

    Function::Function(/*FunctionType* type, */Module& module, std::string_view name)
        : Global(module)
        , mName(name)
        /*,mType(type)*/
    {
    }
}