// Copyright 2023 solar-mist


#include "vipir/Module.h"

#include "vasm/codegen/Elf.h"
#include "vasm/codegen/Pe.h"

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

    void Module::print(std::ostream& stream) const
    {
        stream << std::format("file \"{}\"", mName);
    }

    void Module::emit(std::ostream& stream, OutputFormat format) const
    {
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

        outputFormat->print(stream);
    }
}