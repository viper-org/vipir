// Copyright 2023 solar-mist


#include "vipir/Module.h"

#include "vipir/MC/Builder.h"

#include "vasm/codegen/Elf.h"
#include "vasm/codegen/Pe.h"
#include "vasm/codegen/IOutputFormat.h"
#include "vasm/codegen/builder/OpcodeBuilder.h"

#include <sstream>
#include <format>

namespace vipir
{
    Module::Module(std::string name)
        :mName(std::move(name))
    {
    }

    std::string_view Module::getName() const
    {
        return mName;
    }

    void Module::insertGlobal(Global* global)
    {
        mGlobals.push_back(GlobalPtr(global));
    }

    void Module::print(std::ostream& stream) const
    {
        stream << std::format("file \"{}\"", mName);

        for (const GlobalPtr& global : mGlobals)
        {
            global->print(stream);
        }
    }

    void Module::emit(std::ostream& stream, OutputFormat format) const
    {
        MC::Builder builder;
        for (const GlobalPtr& global : mGlobals)
        {
            global->emit(builder);
        }

        std::unique_ptr<codegen::IOutputFormat> outputFormat;
        switch(format)
        {
            case OutputFormat::ELF:
                outputFormat = std::make_unique<codegen::ELFFormat>(mName);
                break;
            case OutputFormat::PE:
                outputFormat = std::make_unique<codegen::PEFormat>(mName);
                break;
        }

        codegen::OpcodeBuilder opcodeBuilder = codegen::OpcodeBuilder(outputFormat.get(), mName);

        for (const auto& value : builder.getValues())
        {
            value->emit(opcodeBuilder, codegen::Section::Text);
        }

        opcodeBuilder.patchForwardLabels();

        outputFormat->print(stream);
    }
}