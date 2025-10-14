// Copyright 2024 solar-mist


#include "vipir/Optimizer/RegAlloc/VReg.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Memory.h"

namespace vipir
{
    namespace opt
    {
        VReg::VReg(int id, int phys)
            : mId(id)
            , mUses(0)
            , mPhysicalRegister(phys)
            , mOnStack(false)
            , mArgument(false)
            , mSize(0)
        {
        }

        VReg::VReg(int id, int phys, int offset)
            : mId(id)
            , mUses(0)
            , mPhysicalRegister(phys)
            , mStackOffset(offset)
            , mOnStack(true)
            , mArgument(false)
            , mSize(0)
        {
        }

        instruction::OperandPtr VReg::operand(codegen::OperandSize size)
        {
            if (mOnStack)
            {
                instruction::RegisterPtr stack = std::make_unique<instruction::Register>(mPhysicalRegister, codegen::OperandSize::Quad);
                return std::make_unique<instruction::Memory>(std::move(stack), -mStackOffset, nullptr, std::nullopt);
            }
            else
            {
                return std::make_unique<instruction::Register>(mPhysicalRegister, size);
            }
        }

        int VReg::getId() const
        {
            return mId;
        }

        int VReg::getUses() const
        {
            return mUses;
        }

        bool VReg::onStack() const
        {
            return mOnStack;
        }

        int VReg::getPhysicalRegister() const
        {
            return mPhysicalRegister;
        }

        int VReg::getStackOffset() const
        {
            return mStackOffset;
        }
    }
}