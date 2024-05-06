// Copyright 2024 solar-mist


#include "vipir/LIR/Operand.h"

#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/operand/Register.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        Immediate::Immediate(std::intmax_t value)
            : mValue(value)
        {
        }

        std::string Immediate::ident() const
        {
            return std::format("${}", mValue);
        }

        instruction::OperandPtr Immediate::asmOperand()
        {
            return std::make_unique<instruction::Immediate>(mValue);
        }

        OperandPtr Immediate::clone()
        {
            return std::make_unique<Immediate>(mValue);
        }

        bool Immediate::operator==(OperandPtr& other)
        {
            auto imm = dynamic_cast<Immediate*>(other.get());
            if (!imm) return false;

            return imm->mValue == mValue;
        }


        PhysicalReg::PhysicalReg(int id, codegen::OperandSize size)
            : mId(id)
            , mSize(size)
        {
        }

        std::string PhysicalReg::ident() const
        {
            return std::string(codegen::Registers[mId * 4 + static_cast<int>(mSize)]);
        }

        instruction::OperandPtr PhysicalReg::asmOperand()
        {
            return std::make_unique<instruction::Register>(mId, mSize);
        }

        OperandPtr PhysicalReg::clone()
        {
            return std::make_unique<PhysicalReg>(mId, mSize);
        }

        bool PhysicalReg::operator==(OperandPtr& other)
        {
            auto preg = dynamic_cast<PhysicalReg*>(other.get());
            if (!preg)
            {
                auto vreg = dynamic_cast<VirtualReg*>(other.get());
                if (!vreg) return false;
                return vreg->mSize == mSize && !vreg->mVreg->onStack() && vreg->mVreg->getPhysicalRegister() == mId;
            }

            return preg->mId == mId && preg->mSize == mSize;
        }


        VirtualReg::VirtualReg(opt::VReg* vreg, codegen::OperandSize size)
            : mVreg(vreg)
            , mSize(size)
        {
        }

        std::string VirtualReg::ident() const
        {
            return std::format("%T{}", mVreg->getId());
        }

        instruction::OperandPtr VirtualReg::asmOperand()
        {
            return mVreg->operand(mSize);
        }

        OperandPtr VirtualReg::clone()
        {
            return std::make_unique<VirtualReg>(mVreg, mSize);
        }
        
        bool VirtualReg::operator==(OperandPtr& other)
        {
            auto vreg = dynamic_cast<VirtualReg*>(other.get());
            if (!vreg)
            {
                auto preg = dynamic_cast<PhysicalReg*>(other.get());
                if (!preg) return false;
                return mSize == preg->mSize && !mVreg->onStack() && mVreg->getPhysicalRegister() == preg->mId;
            }

            return vreg->mVreg == mVreg && vreg->mSize == mSize;
        }
    }
}