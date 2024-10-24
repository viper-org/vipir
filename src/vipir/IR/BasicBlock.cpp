// Copyright 2024 solar-mist

#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vipir/LIR/Label.h"

#include "vasm/instruction/Label.h"

#include "vasm/instruction/operand/Label.h"

#include <algorithm>
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

    void BasicBlock::insertValue(Value* insertAfter, Value* value)
    {
        if (insertAfter == nullptr)
        {
            insertValue(value);
            return;
        }
        auto it = std::find_if(mValueList.begin(), mValueList.end(), [insertAfter](const auto& value){
            return value.get() == insertAfter;
        });
        if (it == mValueList.end()) mValueList.push_back(ValuePtr(value));
        else mValueList.insert(it+1, ValuePtr(value));
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

    std::vector<BasicBlock*>& BasicBlock::predecessors()
    {
        return mPredecessors;
    }

    BasicBlock*& BasicBlock::loopEnd()
    {
        return mLoopEnd;
    }

    bool BasicBlock::exists()
    {
        return mExists;
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

    void BasicBlock::doConstantFold()
    {
        for (auto& value : mValueList)
        {
            value->doConstantFold();
        }
    }

    void BasicBlock::cleanup()
    {
        for (auto& value : mValueList)
        {
            value->cleanup();
        }
    }

    void BasicBlock::setEmittedValue()
    {
        mEmittedValue = std::make_unique<lir::Lbl>(mName, false);
    }

    void BasicBlock::emit(lir::Builder& builder)
    {
        if (mExists)
        {
            builder.addValue(std::make_unique<lir::Label>(mName, false));
            for (auto& value : mValueList)
            {
                value->emit(builder);
            }
        }
    }

    std::vector<PhiInst*>& BasicBlock::getPhis()
    {
        return mPhis;
    }


    BasicBlock::BasicBlock(std::string_view name, Function* parent)
        : Value(parent->getModule())
        , mName(name)
        , mParent(parent)
        , mLoopEnd(nullptr)
        , mExists(true)
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