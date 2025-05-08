// Copyright 2023 solar-mist


#include "vipir/Module.h"

#include "vipir/Pass/DefaultPass.h"

#include "vipir/IR/Instruction/LoadInst.h"

#include "vipir/IR/Function.h"

#include "vipir/MC/Builder.h"

#include "vipir/Optimizer/RegAlloc/RegAlloc.h"
#include "vipir/Optimizer/Peephole/PeepholeV2.h"

#include "vasm/codegen/Elf.h"
#include "vasm/codegen/Pe.h"
#include "vasm/codegen/IOutputFormat.h"
#include "vasm/codegen/builder/OpcodeBuilder.h"

#include <algorithm>
#include <sstream>
#include <format>

namespace vipir
{
    Module::Module(std::string name)
        : mName(std::move(name))
        , mRunPasses(false)
    {
        mPassManager.addPass(std::make_unique<LIREmissionPass>());
        mPassManager.addPass(std::make_unique<LIRCodegenPass>());
    }

    PassManager& Module::getPassManager()
    {
        return mPassManager;
    }

    void Module::runPasses()
    {
        mPassManager.runPasses(*this);
        mRunPasses = true;
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

    const std::vector<Module::GlobalPtr>& Module::getGlobals() const
    {
        return mGlobals;
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

    void Module::insertGlobalAtFront(Global* global)
    {
        mGlobals.insert(mGlobals.begin(), GlobalPtr(global));
    }

    const std::vector<Module::ValuePtr>& Module::getConstants() const
    {
        return mConstants;
    }

    void Module::insertConstant(Value* constant)
    {
        mConstants.push_back(ValuePtr(constant));
    }

    lir::Builder& Module::getLIRBuilder()
    {
        return mBuilder;
    }

    void Module::print(std::ostream& stream) const
    {
        for (const GlobalPtr& global : mGlobals)
        {
            if (auto func = dynamic_cast<Function*>(global.get()))
            {
                func->orderBasicBlocks();
            }
        }

        stream << std::format("file \"{}\"", mName);

        for (const GlobalPtr& global : mGlobals)
        {
            global->print(stream);
        }
    }


    void Module::printLIR(std::ostream& stream)
    {
        for (auto& value : mBuilder.getValues())
        {
            value->print(stream);
        }
    }

    void Module::setOutputFormat(OutputFormat format)
    {
        switch(format)
        {
            case OutputFormat::ELF:
                mOutputFormat = std::make_unique<codegen::ELFFormat>(mName);
                break;
            case OutputFormat::PE:
                mOutputFormat = std::make_unique<codegen::PEFormat>(mName);
                break;
        }
    }

    void Module::lirEmission()
    {
        for (const GlobalPtr& global : mGlobals)
        {
            if (auto func = dynamic_cast<Function*>(global.get()))
            {
                func->orderBasicBlocks();
            }
        }

        opt::RegAlloc regalloc;
        for (const GlobalPtr& global : mGlobals)
        {
            if (auto func = dynamic_cast<Function*>(global.get()))
            {
                regalloc.assignVirtualRegisters(func, mAbi.get());
            }
        }

        for (const ValuePtr& constant : mConstants)
        {
            constant->emit(mBuilder);
        }

        for (GlobalPtr& global : mGlobals)
        {
            if (auto func = dynamic_cast<Function*>(global.get()))
            {
                func->setEmittedValue();
            }
        }

        for (const GlobalPtr& global : mGlobals)
        {
            global->emit(mBuilder);
        }

        for (auto& global : mGlobals)
        {
            if (auto func = dynamic_cast<Function*>(global.get()))
            {
                func->setCalleeSaved();
            }
        }
    }

    void Module::lirCodegen()
    {
        for (auto& value : mBuilder.getValues())
        {
            value->emit(mInstructionBuilder);
        }

        mOpcodeBuilder = codegen::OpcodeBuilder(mOutputFormat.get(), mName);

        for (const auto& value : mInstructionBuilder.getValues())
        {
            value->emit(mOpcodeBuilder, mOpcodeBuilder.getSection());
        }

        mOpcodeBuilder.patchForwardLabels();
    }

    void Module::generateDebugInfo(DIBuilder* builder)
    {
        builder->generateDwarf(*this, mInstructionBuilder, mOpcodeBuilder);
    }

    void Module::emit(std::ostream& stream)
    {
        if (!mRunPasses) runPasses();
        mOutputFormat->print(stream);
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