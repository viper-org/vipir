// Copyright 2024 solar-mist


#include "vipir/LIR/Operand.h"

#include "vipir/MC/CmpOperand.h"

#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Label.h"

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

        codegen::OperandSize Immediate::size()
        {
            return codegen::OperandSize::Quad;
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

        codegen::OperandSize PhysicalReg::size()
        {
            return mSize;
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

        bool VirtualReg::isMemory()
        {
            return mVreg->onStack();
        }

        codegen::OperandSize VirtualReg::size()
        {
            return mSize;
        }


        Lbl::Lbl(std::string name)
            : mName(std::move(name))
        {
        }

        std::string Lbl::ident() const
        {
            return std::format("LBL @{}", mName);
        }

        instruction::OperandPtr Lbl::asmOperand()
        {
            return std::make_unique<instruction::LabelOperand>(mName);
        }

        OperandPtr Lbl::clone()
        {
            return std::make_unique<Lbl>(mName);
        }

        bool Lbl::operator==(OperandPtr& other)
        {
            auto lbl = dynamic_cast<Lbl*>(other.get());
            if (!lbl) return false;

            return lbl->mName == mName;
        }

        codegen::OperandSize Lbl::size()
        {
            return codegen::OperandSize::None;
        }


        CMP::CMP(Operator op)
            : mOperator(op)
        {
        }

        std::string CMP::ident() const
        {
            return "%CMP";
        }

        instruction::OperandPtr CMP::asmOperand()
        {
            CmpOperator op = static_cast<CmpOperator>(mOperator);
            return std::make_unique<CmpOperand>(op);
        }

        OperandPtr CMP::clone()
        {
            return std::make_unique<CMP>(mOperator);
        }

        bool CMP::operator==(OperandPtr& other)
        {
            auto cmp = dynamic_cast<CMP*>(other.get());
            if (!cmp) return false;

            return cmp->mOperator == mOperator;
        }

        codegen::OperandSize CMP::size()
        {
            return codegen::OperandSize::None;
        }

        std::string CMP::operatorName()
        {
            switch (mOperator)
            {
                case CMP::Operator::EQ:
                    return "EQ";
                case CMP::Operator::NE:
                    return "NE";

                case CMP::Operator::LT:
                    return "LT";
                case CMP::Operator::GT:
                    return "GT";

                case CMP::Operator::LE:
                    return "LE";
                case CMP::Operator::GE:
                    return "GE";
            }
        }
    }
}