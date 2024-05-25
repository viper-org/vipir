// Copyright 2024 solar-mist


#include "vipir/IR/GlobalString.h"

#include "vipir/Module.h"

#include "vipir/LIR/Label.h"
#include "vipir/LIR/Global.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/String.h"

#include "vasm/instruction/singleOperandInstruction/DeclInstruction.h"
#include "vasm/instruction/Directive.h"

#include <format>

namespace vipir
{
    GlobalString* GlobalString::Create(Module& module, std::string value)
    {
        GlobalString* string = new GlobalString(module, std::move(value));
        
        module.insertGlobalAtFront(string);

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


    void GlobalString::emit(lir::Builder& builder)
    {
        builder.setSection(lir::SectionType::Data);
        mEmittedValue = std::make_unique<lir::Lbl>(std::to_string(mValueId), false);

        builder.addValue(std::make_unique<lir::Label>(std::to_string(mValueId), false));
        builder.addValue(std::make_unique<lir::GlobalString>(mValue + '\0'));
    }

    GlobalString::GlobalString(Module& module, std::string value)
        : Global(module)
        , mValue(std::move(value))
        , mValueId(module.getNextValueId())
    {
        mType = Type::GetPointerType(Type::GetIntegerType(8));
    }
}