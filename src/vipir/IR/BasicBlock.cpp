// Copyright 2024 solar-mist

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vipir/LIR/Label.h"

#include "vasm/instruction/Label.h"

#include "vasm/instruction/operand/Label.h"

#include <format>

namespace vipir
{
    BasicBlock* BasicBlock::Create(std::string_view name, Function* parent)
    {
        std::string basicBlockName = std::string(name);
        if (basicBlockName.empty())
        {
            basicBlockName = std::format("{}", parent->getModule().getNextValueId());
        }

        BasicBlock* basicBlock = new BasicBlock(std::move(basicBlockName), parent);
        
        parent->insertBasicBlock(basicBlock);

        return basicBlock;
    }

    Function* BasicBlock::getParent()
    {
        return mParent;
    }

    void BasicBlock::insertValue(Value* value)
    {
        mValueList.push_back(ValuePtr(value));
    }

    void BasicBlock::eraseValue(Value* value)
    {
        std::erase_if(mValueList, [value](ValuePtr& valuePtr) {
            return valuePtr.get() == value;
        });
    }

    std::vector<Value*>& BasicBlock::liveIn()
    {
        return mLiveIn;
    }

    std::vector<BasicBlock*>& BasicBlock::successors()
    {
        return mSuccessors;
    }

    BasicBlock*& BasicBlock::loopEnd()
    {
        return mLoopEnd;
    }

    int& BasicBlock::endPosition()
    {
        return mEndPosition;
    }

    void BasicBlock::print(std::ostream& stream)
    {
        stream << std::format("{}:\n", mName);
        for (auto& value : mValueList)
        {
            stream << "\t";
            value->print(stream);
            stream << "\n";
        }
    }

    std::string BasicBlock::ident() const
    {
        return std::format("label %{}", mName);
    }

    void BasicBlock::setEmittedValue()
    {
        mEmittedValue = std::make_unique<lir::Lbl>(mName, false);
    }

    void BasicBlock::emit(lir::Builder& builder)
    {
        builder.addValue(std::make_unique<lir::Label>(mName, false));
        for (auto& value : mValueList)
        {
            value->emit(builder);
        }
    }


    BasicBlock::BasicBlock(std::string_view name, Function* parent)
        : Value(parent->getModule())
        , mName(name)
        , mParent(parent)
        , mLoopEnd(nullptr)
    {
        bool alldigits = true;
        for (char c : mName)
        {
            if (!std::isdigit(c)) 
            {
                alldigits = false;
                break;
            }
        }
        if (alldigits) mName = ".L" + mName;
    }
}