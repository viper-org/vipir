// Copyright 2024 solar-mist

#ifndef VIPIR_OPTIMIZER_REGALLOC_VREG_H
#define VIPIR_OPTIMIZER_REGALLOC_VREG_H 1

#include "vasm/instruction/Operand.h"
#include "vasm/codegen/Opcodes.h"

namespace vipir
{
    namespace opt
    {
        class RegAlloc;

        class VReg
        {
        friend class RegAlloc;
        public:
            VReg() = default;
            VReg(int id, int phys);
            VReg(int id, int phys, int stackOffset);

            instruction::OperandPtr operand(codegen::OperandSize size);

            int getId() const;
        private:
            int mId;

            bool mOnStack;

            int mPhysicalRegister;
            int mStackOffset;

            int mSize;
        };
    }
}

#endif // VIPIR_OPTIMIZER_REGALLOC_VREG_H