// Copyright 2024 solar-mist


#include "vipir/IR/Argument.h"

#include "vipir/Module.h"

#include "vipir/LIR/Operand.h"

#include "vasm/instruction/operand/Register.h"

namespace vipir
{
    Argument::Argument(Module& module, Type* type, std::string&& name, int index)
        : Value(module)
        , mName(std::move(name))
        , mIdx(index)
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

    void Argument::emit(MC::Builder&)
    {
        mEmittedValue = mVReg->operand(mType->getOperandSize());
    }

    void Argument::emit2(lir::Builder&)
    {
        int registerId = mModule.abi()->getParameterRegister(mIdx);
        mEmittedValue2 = std::make_unique<lir::PhysicalReg>(registerId, mType->getOperandSize());
    }
}