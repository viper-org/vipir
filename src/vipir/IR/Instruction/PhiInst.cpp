// Copyright 2024 solar-mist

#include "vipir/IR/Instruction/PhiInst.h"
#include "vipir/IR/BasicBlock.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/Move.h"

namespace vipir
{
    void PhiInst::print(std::ostream& stream)
    {
        stream << std::format("%{} = phi [ ", getName(mValueId));
        for (size_t i = 0; i < mIncoming.size() - 1; ++i)
        {
            stream << std::format("{} : {} ]", mIncoming[i].first->ident(), mIncoming[i].second->ident());
        }
        stream << std::format("{} : {} ]", mIncoming.back().first->ident(), mIncoming.back().second->ident());
    }

    std::vector<Value*> PhiInst::getOperands()
    {
        std::vector<Value*> operands;
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

        for (auto& incoming : mIncoming)
        {
            auto position = incoming.second->endPosition() + builder.getInsertCount();
            builder.insertValue(std::make_unique<lir::Move>(vreg->clone(), incoming.first->getEmittedValue()), position);
        }
        mEmittedValue = std::move(vreg);
    }

    std::string PhiInst::ident() const
    {
        return getName(mValueId);
    }


    PhiInst::PhiInst(BasicBlock* parent, Type* type)
        : Instruction(parent->getModule(), parent)
        , mValueId(mModule.getNextValueId())
    {
        mType = type;
    }
}