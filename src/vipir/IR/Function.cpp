// Copyright 2024 solar-mist

#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vipir/LIR/Label.h"
#include "vipir/LIR/Instruction/EnterFunc.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/Directive.h"
#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Label.h"

#include "vasm/instruction/singleOperandInstruction/PushInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"

#include <algorithm>
#include <deque>
#include <format>

namespace vipir
{
    Function* Function::Create(FunctionType* type, Module& module, std::string_view name)
    {
        Function* func = new Function(type, module, name);

        module.insertGlobal(func);

        return func;
    }

    FunctionType* Function::getFunctionType() const
    {
        return static_cast<FunctionType*>(mType);
    }

    Argument* Function::getArgument(int index) const
    {
        if (index < mArguments.size()) return mArguments.at(index).get();

        return nullptr;
    }

    bool Function::usesStack() const
    {
        return mTotalStackOffset > 0;
    }

    void Function::insertBasicBlock(BasicBlock* basicBlock)
    {
        mBasicBlockList.push_back(BasicBlockPtr(basicBlock));
    }

    void Function::print(std::ostream& stream)
    {
        stream << std::format("\n\nfunction @{}() -> {} ", mName, getFunctionType()->getReturnType()->getName());
        if (!mBasicBlockList.empty())
        {
            stream << "{\n";
            for (auto& basicBlock : mBasicBlockList)
            {
                basicBlock->print(stream);
            }
            stream << "}";
        }
    }

    std::string Function::ident() const
    {
        return "@" + mName;
    }

    void Function::emit(lir::Builder& builder)
    {
        builder.setSection(lir::SectionType::Code);

        if (mBasicBlockList.empty())
        {
            mEmittedValue = std::make_unique<lir::Lbl>(mName);
            builder.addValue(std::make_unique<lir::ExternLabel>(mName));
            return;
        }

        mEmittedValue = std::make_unique<lir::Lbl>(mName);
        builder.addValue(std::make_unique<lir::Label>(mName, true));

        if (mTotalStackOffset > 0) // There are local variables
        {
            builder.addValue(std::make_unique<lir::EnterFunc>(mTotalStackOffset));
        }

        for (auto& basicBlock : mBasicBlockList)
        {
            basicBlock->setEmittedValue();
        }

        for (auto& argument : mArguments)
        {
            argument->emit(builder);
        }

        for (auto& basicBlock : mBasicBlockList)
        {
            basicBlock->emit(builder);
        }
    }

    Function::Function(FunctionType* type, Module& module, std::string_view name)
        : Global(module)
        , mName(name)
        , mTotalStackOffset(0)
    {
        mType = type;

        int index = 0;
        for (auto type : getFunctionType()->getArgumentTypes())
        {
            std::string id = std::to_string(module.getNextValueId());
            mArguments.push_back(std::make_unique<Argument>(module, type, std::move(id), index));
        }
    }
}