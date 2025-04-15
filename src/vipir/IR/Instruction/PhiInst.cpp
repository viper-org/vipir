// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/PhiInst.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"
#include "vipir/LIR/Instruction/LoadAddress.h"

#include <algorithm>

namespace vipir
{
    void PhiInst::print(std::ostream& stream)
    {
        stream << std::format("%{} = phi [ ", getName(mValueId));
        for (size_t i = 0; i < mIncoming.size() - 1; ++i)
        {
            stream << std::format("{} : {}, ", mIncoming[i].first->ident(), mIncoming[i].second->ident());
        }
        stream << std::format("{} : {} ]", mIncoming.back().first->ident(), mIncoming.back().second->ident());
    }

    std::vector<std::reference_wrapper<Value*> > PhiInst::getOperands()
    {
        std::vector<std::reference_wrapper<Value*> > operands;
        for (auto& incoming : mIncoming)
        {
            operands.push_back(incoming.first);
        }
        return operands;
    }
    
    void PhiInst::eraseFromParent()
    {
        std::erase(mParent->mPhis, this);
        
        Instruction::eraseFromParent();
    }

    void PhiInst::addIncoming(Value* value, BasicBlock* basicBlock)
    {
        mIncoming.push_back(std::make_pair(value, basicBlock));
    }

    void PhiInst::emit(lir::Builder& builder)
    {
        std::vector<BasicBlock*> done;
        for (auto& incoming : mIncoming)
        {
            if (std::find(done.begin(), done.end(), incoming.second) != done.end()) continue;
            if (incoming.second->exists())
            {
                if (incoming.first == this) continue;

                done.push_back(incoming.second);
                auto value = std::make_unique<lir::Move>(mEmittedValue->clone(), incoming.first->getEmittedValue());
                auto ptr = value.get();
                builder.insertValue(std::move(value), incoming.second->endNode());
                incoming.second->endNode() = ptr;
            }
        }
    }

    void PhiInst::setEmittedValue()
    {
        mEmittedValue = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
    }

    std::string PhiInst::ident() const
    {
        return "%" + getName(mValueId);
    }

    void PhiInst::doConstantFold()
    {
    }


    PhiInst::PhiInst(BasicBlock* parent, Type* type, AllocaInst* alloca)
        : Instruction(parent->getModule(), parent)
        , mValueId(mModule.getNextValueId())
        , mAlloca(alloca)
    {
        mParent->getPhis().push_back(this);
        mType = type;
    }
}