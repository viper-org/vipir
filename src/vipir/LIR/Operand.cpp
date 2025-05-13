// Copyright 2024 solar-mist


#include "vipir/LIR/Operand.h"

#include "vipir/MC/CmpOperand.h"
#include "vipir/MC/CompoundOperand.h"

#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/Relative.h"
#include "vasm/instruction/operand/Memory.h"

#include <cassert>
#include <format>
#include <iostream>

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
        
        std::intmax_t Immediate::value() const
        {
            return mValue;
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
                return !vreg->mVreg->onStack() && vreg->mVreg->getPhysicalRegister() == mId;
            }

            return preg->mId == mId;
        }

        codegen::OperandSize PhysicalReg::size()
        {
            return mSize;
        }


        VirtualReg::VirtualReg(opt::VReg* vreg, codegen::OperandSize size, bool pointer)
            : mVreg(vreg)
            , mSize(size)
            , mPointer(pointer)
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
            return std::make_unique<VirtualReg>(mVreg, mSize, mPointer);
        }
        
        bool VirtualReg::operator==(OperandPtr& other)
        {
            auto vreg = dynamic_cast<VirtualReg*>(other.get());
            if (!vreg)
            {
                auto preg = dynamic_cast<PhysicalReg*>(other.get());
                if (!preg) return false;
                return !mVreg->onStack() && mVreg->getPhysicalRegister() == preg->mId;
            }

            return vreg->mVreg == mVreg;
        }

        bool VirtualReg::isMemory()
        {
            return mVreg->onStack();
        }

        bool VirtualReg::isPointer()
        {
            return mPointer;
        }

        codegen::OperandSize VirtualReg::size()
        {
            return mSize;
        }

        bool& VirtualReg::pointer()
        {
            return mPointer;
        }


        Lbl::Lbl(std::string name, bool plt)
            : mName(std::move(name))
            , mPlt(plt)
        {
        }

        std::string Lbl::ident() const
        {
            return std::format("LBL @{}", mName);
        }

        instruction::OperandPtr Lbl::asmOperand()
        {
            return std::make_unique<instruction::LabelOperand>(mName, mPlt ? "plt" : "");
        }

        OperandPtr Lbl::clone()
        {
            return std::make_unique<Lbl>(mName, mPlt);
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

        std::string Lbl::getName()
        {
            return mName;
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


        Memory::Memory(codegen::OperandSize size, OperandPtr base, std::optional<int> displacement, OperandPtr index, std::optional<int> scale, bool pointer)
            : mSize(size)
            , mBase(std::move(base))
            , mDisplacement(displacement)
            , mIndex(std::move(index))
            , mScale(scale)
            , mPointer(pointer)
        {
        }

        std::string Memory::ident() const
        {
            if (mDisplacement)
            {
                if (mIndex)
                {
                    auto scale = mScale.value_or(1);
                    return std::format("({}+{}*{}+{}", mBase->ident(), mIndex->ident(), scale, *mDisplacement);
                }
                return std::format("({}+{}", mBase->ident(), *mDisplacement);
            }
            if (mIndex)
            {
                auto scale = mScale.value_or(1);
                return std::format("({}+{}*{}", mBase->ident(), mIndex->ident(), scale);
            }
            return std::format("({})", mBase->ident());
        }

        instruction::OperandPtr Memory::asmOperand()
        {
            instruction::OperandPtr baseOperand = mBase->asmOperand();
            if (auto label = dynamic_cast<instruction::LabelOperand*>(baseOperand.get()))
            {
                std::string name = std::string(label->getName());
                std::string location = std::string(label->getLocation());
                instruction::LabelOperandPtr labelPtr = std::make_unique<instruction::LabelOperand>(std::move(name), std::move(location));
                return std::make_unique<instruction::Relative>(std::move(labelPtr), mDisplacement);
            }

            instruction::RegisterPtr base;
            std::optional<int> displacement = mDisplacement;
            instruction::RegisterPtr index = nullptr;
            std::optional<int> scale = mScale;

            if (auto reg = dynamic_cast<instruction::Register*>(baseOperand.get()))
            {
                base = instruction::RegisterPtr(static_cast<instruction::Register*>(baseOperand.release()));
            }
            else if (auto mem = dynamic_cast<instruction::Memory*>(baseOperand.get()))
            {
                if (mIndex == nullptr && mScale.value_or(0) == 0)
                {
                    if (mem->getIndex())
                        index = mem->getIndex()->clone(mem->getIndex()->getSize());
                    scale = mem->getScale();
                }
                else
                {
                    assert(mem->getIndex() == nullptr);
                    //assert(mem->getScale().value_or(0) == 0);
                }

                base = mem->getBase()->clone(mem->getBase()->getSize());
                if (mem->getDisplacement())
                {
                    if (displacement) *displacement += *mem->getDisplacement();
                    else displacement = *mem->getDisplacement();
                }
            }

            if (mIndex)
            {
                instruction::OperandPtr indexOperand = mIndex->asmOperand();
                if (auto imm = dynamic_cast<instruction::Immediate*>(indexOperand.get()))
                {
                    if (displacement) *displacement += imm->imm32() * mScale.value_or(1);
                    else displacement = imm->imm32() * mScale.value_or(1);
                }
                else
                {
                    index = instruction::RegisterPtr(static_cast<instruction::Register*>(indexOperand.release()));
                }
            }

            return std::make_unique<instruction::Memory>(std::move(base), displacement, std::move(index), scale);
        }

        OperandPtr Memory::clone()
        {
            lir::OperandPtr index;
            if (mIndex) index = mIndex->clone();
            return std::make_unique<Memory>(mSize, mBase->clone(), mDisplacement, std::move(index), mScale, mPointer);
        }

        bool Memory::operator==(OperandPtr& other)
        {
            auto mem = dynamic_cast<Memory*>(other.get());
            if (!mem) return false;

            return *mem->mBase == mBase && mem->mDisplacement == mDisplacement && *mem->mIndex == mIndex && mem->mScale == mScale;
        }

        bool Memory::isMemory()
        {
            return true;
        }

        bool Memory::isPointer()
        {
            return mPointer;
        }

        codegen::OperandSize Memory::size()
        {
            return mSize;
        }

        OperandPtr Memory::base()
        {
            return mBase->clone();
        }

        void Memory::addDisplacement(int displacement)
        {
            if (mDisplacement)
            {
                *mDisplacement += displacement;
            }
            else
            {
                mDisplacement = displacement;
            }
        }


        Compound::Compound(std::vector<OperandPtr> values)
            : mValues(std::move(values))
        {
        }

        std::string Compound::ident() const
        {
            std::string ret = "COMPOUND { ";
            for (auto& value : mValues)
            {
                ret += value->ident() + " ";
            }
            return ret + "}";
        }

        instruction::OperandPtr Compound::asmOperand()
        {
            std::vector<instruction::OperandPtr> values;
            for (auto& value : mValues)
            {
                values.push_back(value->asmOperand());
            }

            return std::make_unique<CompoundOperand>(std::move(values));
        }

        OperandPtr Compound::clone()
        {
            std::vector<OperandPtr> values;
            for (auto& value : mValues)
            {
                values.push_back(value->clone());
            }

            return std::make_unique<Compound>(std::move(values));
        }

        bool Compound::operator==(OperandPtr& other)
        {
            auto compound = dynamic_cast<Compound*>(other.get());
            if (!compound) return false;

            if (mValues.size() != compound->mValues.size()) return false;

            for (int i = 0; i < mValues.size(); ++i)
            {
                if (*mValues[i] != compound->mValues[i]) return false;
            }
            return true;
        }

        codegen::OperandSize Compound::size()
        {
            return codegen::OperandSize::None;
        }

        std::vector<OperandPtr>& Compound::getValues()
        {
            return mValues;
        }
    }
}