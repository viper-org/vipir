// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        Move::Move(OperandPtr left, OperandPtr right)
            : mLeft(std::move(left))
            , mRight(std::move(right))
        {
        }

        void Move::print(std::ostream& stream) const
        {
            stream << std::format("MOVE {} -> {}\n", mRight->ident(), mLeft->ident());
        }

        void Move::emit(MC::Builder& builder)
        {
            if (*mLeft == mRight) return;
            builder.addValue(std::make_unique<instruction::MovInstruction>(mLeft->asmOperand(), mRight->asmOperand()));
        }
    }
}