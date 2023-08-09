// Copyright 2023 solar-mist


#include "vipir/IR/Function.h"
#include "vipir/IR/BasicBlock.h"

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

    int& Function::getInstructionCount()
    {
        return mInstructionCount;
    }
    
    std::string_view Function::getName() const
    {
        return mName;
    }

    const std::vector<BasicBlockPtr>& Function::getBasicBlockList() const
    {
        return mBasicBlockList;
    }

    void Function::insertBasicBlock(BasicBlock* basicBlock)
    {
        mBasicBlockList.push_back(BasicBlockPtr(basicBlock));
    }

    void Function::print(std::ostream& stream) const
    {
        stream << std::format("define pub void @{}() {{\n   ", mName);

        for (const BasicBlockPtr& basicBlock : mBasicBlockList)
        {
            basicBlock->print(stream);
        }

        stream << "}";
    }

    std::string Function::ident() const
    {
        return std::format("@{}", mName);
    }


    void Function::emit(std::ostream& stream) const
    {
        stream << std::format("{}:\n", mName);

        for (const BasicBlockPtr& basicBlock : mBasicBlockList)
        {
            basicBlock->emit(stream);
        }
    }


    Function::Function(Module& module, std::string name)
        :Global(module), mName(std::move(name))
    {
    }
}