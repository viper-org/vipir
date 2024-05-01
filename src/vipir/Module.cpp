// Copyright 2023 solar-mist


#include "vipir/Module.h"

#include "vipir/IR/Instruction/LoadInst.h"
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

    abi::ABI* Module::abi() const
    {
        return mAbi.get();
    }

    std::string_view Module::getName() const
    {
        return mName;
    }

    int Module::getNextValueId()
    {
        static int valueId = 0;
        return ++valueId;
    }

    GlobalVar* Module::createGlobalVar(Type* type)
    {
        GlobalVar* global = new GlobalVar(*this, type);
        insertGlobalAt(global, -1);
        return global;
    }

    void Module::insertGlobal(Global* global)
    {
        mGlobals.push_back(GlobalPtr(global));
    }
    void Module::insertGlobalAt(Global* global, int offset)
    {
        if (mGlobals.empty())
        {
            mGlobals.push_back(GlobalPtr(global));
        }
        else
        {
            mGlobals.insert(mGlobals.end() + offset, GlobalPtr(global));
        }
    }

    void Module::insertConstant(Value* constant)
    {
        mConstants.push_back(ValuePtr(constant));
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
        for (const ValuePtr& constant : mConstants)
        {
            constant->emit(builder);
        }
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
            value->emit(opcodeBuilder, opcodeBuilder.getSection());
        }

        opcodeBuilder.patchForwardLabels();

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