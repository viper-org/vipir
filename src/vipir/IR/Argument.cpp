// Copyright 2024 solar-mist


#include "vipir/IR/Argument.h"

#include "vipir/Module.h"

#include "vipir/LIR/Operand.h"
#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/operand/Register.h"

namespace vipir
{
    Argument::Argument(Module& module, Type* type, std::string&& name, int index)
        : Value(module)
        , mName(std::move(name))
        , mIdx(index)
        , mMoveTo(nullptr)
    {
        mType = type;
    }

    void Argument::print(std::ostream&)
    {
    }

    std::string Argument::ident() const
    {
        return "%" + mName;
    }

    void Argument::doConstantFold()
    {
    }

    void Argument::emit(lir::Builder& builder)
    {
        if (mMoveTo)
        {
            auto source = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
            mEmittedValue = std::make_unique<lir::VirtualReg>(mMoveTo, mType->getOperandSize());
            builder.addValue(std::make_unique<lir::Move>(mEmittedValue->clone(), std::move(source)));
        }
        else
        {
            if (mVReg->onStack())
            {
                mEmittedValue = std::make_unique<lir::Memory>(mType->getOperandSize(), std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize()), std::nullopt, nullptr, std::nullopt);
            }
            else
            {
                mEmittedValue = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
            }
        }
    }
}