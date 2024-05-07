// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/Relative.h"

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

            instruction::OperandPtr left = mLeft->asmOperand();
            if (auto label = dynamic_cast<instruction::LabelOperand*>(left.get()))
            {
                (void)left.release();
                instruction::LabelOperandPtr lbl = instruction::LabelOperandPtr(label);
                left = std::make_unique<instruction::Relative>(std::move(lbl), std::nullopt);
            }
            builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(left), mRight->asmOperand(), mLeft->size()));
        }
    }
}