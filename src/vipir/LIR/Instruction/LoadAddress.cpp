// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/LoadAddress.h"

#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"

#include <cassert>
#include <format>

namespace vipir
{
    namespace lir
    {
        LoadAddress::LoadAddress(OperandPtr left, OperandPtr right)
            : mLeft(std::move(left))
            , mRight(std::move(right))
        {
            assert(mRight->isMemory());
        }

        void LoadAddress::print(std::ostream& stream) const
        {
            stream << std::format("LA {} -> {}\n", mRight->ident(), mLeft->ident());
        }

        void LoadAddress::emit(MC::Builder& builder)
        {
            builder.addValue(std::make_unique<instruction::LeaInstruction>(mLeft->asmOperand(), mRight->asmOperand()));
        }
    }
}