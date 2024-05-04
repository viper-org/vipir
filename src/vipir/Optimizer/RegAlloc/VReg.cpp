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
            , mPhysicalRegister(phys)
            , mOnStack(false)
        {
        }

        VReg::VReg(int id, int phys, int offset)
            : mId(id)
            , mPhysicalRegister(phys)
            , mStackOffset(offset)
            , mOnStack(true)
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
    }
}