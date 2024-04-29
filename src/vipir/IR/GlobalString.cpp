// Copyright 2024 solar-mist


#include "vipir/IR/GlobalString.h"

#include "vipir/Module.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/String.h"

#include "vasm/instruction/singleOperandInstruction/DeclInstruction.h"

#include <format>

namespace vipir
{
    GlobalString* GlobalString::Create(Module& module, std::string value)
    {
        GlobalString* string = new GlobalString(module, std::move(value));
        
        module.insertGlobalAt(string, -1);

        return string;
    }

    void GlobalString::print(std::ostream& stream)
    {
        stream << std::format("\n\nstr @{} = \"{}\"", mValueId, mValue);
    }

    std::string GlobalString::ident() const
    {
        return std::format("str @{}", mValueId);
    }

    void GlobalString::emit(MC::Builder& builder)
    {
        mEmittedValue = std::make_unique<instruction::LabelOperand>(std::to_string(mValueId));

        builder.addValue(std::make_unique<instruction::Label>(std::to_string(mValueId)));
        instruction::OperandPtr string = std::make_unique<instruction::String>(mValue);
        builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Byte> >(std::move(string)));
    }

    GlobalString::GlobalString(Module& module, std::string value)
        : Global(module)
        , mValue(std::move(value))
        , mValueId(module.getNextValueId())
    {
        mType = Type::GetPointerType(Type::GetIntegerType(8));
    }
}