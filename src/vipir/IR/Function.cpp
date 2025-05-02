// Copyright 2024 solar-mist

#include "vipir/IR/Function.h"

#include "vipir/Module.h"

#include "vipir/IR/Instruction/PhiInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"

#include "vipir/LIR/Label.h"
#include "vipir/LIR/Instruction/EnterFunc.h"
#include "vipir/LIR/Instruction/Jump.h"
#include "vipir/LIR/Instruction/Ret.h"

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
#include <stack>

namespace vipir
{
    Function* Function::Create(FunctionType* type, Module& module, std::string_view name, bool pure, const abi::CallingConvention* callingConvention)
    {
        Function* func = new Function(type, module, name, pure, callingConvention);

        module.insertGlobal(func);

        return func;
    }

    Function* Function::Create(FunctionType* type, Module& module, std::string_view name, bool pure)
    {
        return Function::Create(type, module, name, pure, module.abi()->getDefaultCallingConvention());
    }
    
    void Function::replaceAllUsesWith(Value* old, Value* newValue)
    {
        std::vector<Value*> done;
        for (auto& bb : mBasicBlockList)
        {
            for (auto& value : bb->mValueList)
            {
                //if (std::find(done.begin(), done.end(), value.get()) == done.end())
                {
                    done.push_back(value.get());
                    for (auto operand : value->getOperands())
                    {
                        if (operand == old)
                        {
                            operand.get() = newValue;
                        }
                    }
                }
            }
        }
    }

    const abi::CallingConvention* Function::getCallingConvention() const
    {
        return mCallingConvention;
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

    std::vector<int> Function::getCalleeSaved()
    {
        return mCalleeSaved;
    }

    void Function::insertBasicBlock(BasicBlock* basicBlock)
    {
        mBasicBlockList.push_back(BasicBlockPtr(basicBlock));
    }

    int Function::getNumBasicBlocks()
    {
        return mBasicBlockList.size();
    }

    std::vector<std::unique_ptr<BasicBlock> >& Function::getBasicBlocks()
    {
        return mBasicBlockList;
    }

    void Function::print(std::ostream& stream)
    {
        stream << std::format("\n\nfunction @{}(", mName);
        if (!mArguments.empty())
        {
            for (int i = 0; i < mArguments.size()-1; ++i)
            {
                stream << std::format("{} {}, ", mArguments[i]->mType->getName(), mArguments[i]->ident());
            }
            stream << std::format("{} {}", mArguments.back()->mType->getName(), mArguments.back()->ident());
        }
        stream << std::format(") -> {} ", getFunctionType()->getReturnType()->getName());

        if (!mBasicBlockList.empty())
        {
            stream << "{\n";
            for (auto& basicBlock: mBasicBlockList)
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

    void Function::doConstantFold()
    {
        for (auto& basicBlock: mBasicBlockList)
        {
            basicBlock->doConstantFold();
        }
    }

    void Function::setDebugEndLocation(int endLine, int endCol)
    {
        mEndLine = endLine;
        mEndCol = endCol;
    }

    void Function::setEmittedValue()
    {
        bool plt = mBasicBlockList.empty();

        mEmittedValue = std::make_unique<lir::Lbl>(mName, plt);
    }

    bool Function::isPure() const
    {
        return mIsPure;
    }

    void Function::emit(lir::Builder& builder)
    {
        builder.setSection(lir::SectionType::Code);

        if (mBasicBlockList.empty())
        {
            builder.addValue(std::make_unique<lir::ExternLabel>(mName));
            return;
        }
        
        builder.addValue(std::make_unique<lir::Label>(mName, true));

        if (mLine)
        {
            builder.addValue(std::make_unique<lir::EmitSourceInfo>(mLine, mCol));
        }

        builder.addValue(std::make_unique<lir::EnterFunc>(mTotalStackOffset, mCalleeSaved));
        mEnterFuncNode = builder.getValues().back().get();

        lir::Builder newBuilder;

        for (auto& basicBlock: mBasicBlockList)
        {
            basicBlock->setEmittedValue();
        }

        for (auto& argument: mArguments)
        {
            argument->emit(newBuilder);
        }

        for (auto& basicBlock: mBasicBlockList)
        {
            basicBlock->emit(newBuilder);
        }
        for (auto& basicBlock: mBasicBlockList)
        {
            for (auto phi : basicBlock->getPhis())
            {
                phi->emit(newBuilder);
            }
        }


        for (auto& value: newBuilder.getValues())
        {
            if (auto ret = dynamic_cast<lir::Ret*>(value.get()))
            {
                mRetNodes.push_back(ret);
            }
            if (auto call = dynamic_cast<lir::Call*>(value.get()))
            {
                mHasCallNodes = true;
            }
        }
        std::move(newBuilder.getValues().begin(), newBuilder.getValues().end(), std::back_inserter(builder.getValues()));

        if (mEndLine)
        {
            builder.addValue(std::make_unique<lir::EmitSourceInfo>(mEndLine, mEndCol));
        }
    }

    static void postorder(BasicBlock* head, std::vector<BasicBlock*>& visited, std::stack<BasicBlock*>& stack)
    {
        if (std::find(visited.begin(), visited.end(), head) != visited.end()) return;
        visited.push_back(head);
        for (auto succ : head->successors())
        {
            postorder(succ, visited, stack);
        }
        stack.push(head);
    }

    void Function::orderBasicBlocks()
    {
        if (mBasicBlockList.empty()) return;
        std::vector<BasicBlock*> visited;
        std::stack<BasicBlock*> stack;
        postorder(mBasicBlockList[0].get(), visited, stack);

        std::vector<BasicBlock*> blocks;
        for (auto& bb : mBasicBlockList)
        {
            blocks.push_back(bb.release());
        }
        mBasicBlockList.clear();
        while (!stack.empty())
        {
            auto bb = stack.top();
            stack.pop();
            mBasicBlockList.push_back(BasicBlockPtr(bb));
            std::erase(blocks, bb);
        }
        // The basicblocks need to still exist, just not be in the function anymore 
        for (auto bb : blocks)
        {
            bb->mExists = false;
            mBasicBlockList.push_back(BasicBlockPtr(bb));
        }
    }

    std::vector<AllocaInst*> Function::getAllocaList()
    {
        std::vector<AllocaInst*> ret;
        for (auto& basicBlock: mBasicBlockList)
        {
            for (auto& value: basicBlock->mValueList)
            {
                if (auto alloca = dynamic_cast<AllocaInst*>(value.get())) ret.push_back(alloca);
            }
        }
        return ret;
    }

    Function::Function(FunctionType* type, Module& module, std::string_view name, bool pure, const abi::CallingConvention* callingConvention)
            : Global(module)
            , mCallingConvention(callingConvention)
            , mName(callingConvention->decorateName(name, type))
            , mTotalStackOffset(0)
            , mHasCallNodes(false)
            , mIsPure(pure)
    {
        mType = type;

        int index = 0;
        for (auto type: getFunctionType()->getArgumentTypes())
        {
            std::string id = std::to_string(module.getNextValueId());
            mArguments.push_back(std::make_unique<Argument>(module, type, std::move(id), index++));
        }
    }

    void Function::setCalleeSaved()
    {
        if (mBasicBlockList.empty()) return;
        mCalleeSaved.clear();
        std::vector<int> abiCalleeSaved = mCallingConvention->getCalleeSavedRegisters();
        for (auto& vreg: mVirtualRegs)
        {
            if (!vreg->onStack() && vreg->getUses() > 0)
            {
                auto it = std::find(abiCalleeSaved.begin(), abiCalleeSaved.end(), vreg->getPhysicalRegister());
                if (it != abiCalleeSaved.end()) mCalleeSaved.push_back(vreg->getPhysicalRegister());
            }
        }
        if (mCalleeSaved.size() % 2 != 0)
        { // Keep stack aligned to 16 bytes
            mTotalStackOffset += 8;
        }

        lir::EnterFunc* node = static_cast<lir::EnterFunc*>(mEnterFuncNode);
        node->setStackSize(mTotalStackOffset);
        node->setCalleeSaved(mCalleeSaved);
        node->setSaveFramePointer(mHasCallNodes || mTotalStackOffset);
        for (auto node: mRetNodes)
        {
            lir::Ret* ret = static_cast<lir::Ret*>(node);
            ret->setLeave(mTotalStackOffset > 0);
            ret->setStackSize(mTotalStackOffset);
            ret->setCalleeSaved(mCalleeSaved);
            ret->setSaveFramePointer(mHasCallNodes || mTotalStackOffset);
        }
    }
}