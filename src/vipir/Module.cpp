// Copyright 2023 solar-mist


#include "vipir/Module.h"

#include "vipir/IR/Global.h"

#include "vasm/codegen/Elf.h"
#include "vasm/codegen/IOutputFormat.h"
#include "vasm/codegen/builder/OpcodeBuilder.h"
#include "vipir/IR/Instruction/LoadInst.h"

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

    const std::vector<GlobalPtr>& Module::getGlobals() const
    {
        return mGlobals;
    }

    void Module::insertGlobal(Global* global)
    {
        mGlobals.push_back(GlobalPtr(global));
    }

    instruction::OperandPtr Module::getGlobalEmittedValue(ValueId id)
    {
        return mGlobals.at(id)->getEmittedValue();
    }

    void Module::print(std::ostream& stream) const
    {
        stream << std::format("file \"{}\"", mName);

        for (const GlobalPtr& global : mGlobals)
        {
            global->print(stream);
        }
    }

    void Module::optimize(OptimizationLevel level)
    {
        if (level == OptimizationLevel::None)
        {
            return;
        }

        for (auto& global : mGlobals)
        {
            global->optimize(level);
        }
    }

    void Module::emit(std::ostream& stream) const
    {
        std::vector<instruction::ValuePtr> values;
        for (const GlobalPtr& global : mGlobals)
        {
            global->emit(values);
        }

        std::unique_ptr<codegen::IOutputFormat> outputFormat = std::make_unique<codegen::ELFFormat>(mName);
        codegen::OpcodeBuilder builder = codegen::OpcodeBuilder(outputFormat.get());

        for (const auto& value : values)
        {
            value->emit(builder, codegen::Section::Text);
        }

        builder.patchForwardLabels();

        outputFormat->print(stream);
    }

    Value* getPointerOperand(Value* value)
    {
        if (LoadInst* load = dynamic_cast<LoadInst*>(value))
        {
            return load->getPointer();
        }
        return nullptr;
    }
}