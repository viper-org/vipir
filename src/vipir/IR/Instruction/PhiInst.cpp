// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/PhiInst.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"

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

    void PhiInst::addIncoming(Value* value, BasicBlock* basicBlock)
    {
        mIncoming.push_back(std::make_pair(value, basicBlock));
    }

    void PhiInst::emit(lir::Builder& builder)
    {
        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());

        std::vector<BasicBlock*> done;
        for (auto& incoming : mIncoming)
        {
            if (std::find(done.begin(), done.end(), incoming.second) != done.end()) continue;
            if (incoming.second->exists())
            {
                done.push_back(incoming.second);
                auto position = incoming.second->endPosition();
                position += builder.getInsertsBefore(position);
                builder.insertValue(std::make_unique<lir::Move>(vreg->clone(), incoming.first->getEmittedValue()), position);
            }
        }
        mEmittedValue = std::move(vreg);
    }

    std::string PhiInst::ident() const
    {
        return getName(mValueId);
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