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
            instruction::OperandPtr right = mRight->asmOperand();
            if (auto label = dynamic_cast<instruction::LabelOperand*>(left.get()))
            {
                (void)left.release();
                instruction::LabelOperandPtr lbl = instruction::LabelOperandPtr(label);
                left = std::make_unique<instruction::Relative>(std::move(lbl), std::nullopt);
            }
            if (dynamic_cast<instruction::Memory*>(left.get()) && dynamic_cast<instruction::Memory*>(right.get()))
            {
                instruction::OperandPtr reg = std::make_unique<instruction::Register>(0, mRight->size());
                builder.addValue(std::make_unique<instruction::MovInstruction>(reg->clone(), std::move(right)));
                right = std::move(reg);
            }
            builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(left), std::move(right), mLeft->size()));
        }


        MoveSX::MoveSX(OperandPtr left, OperandPtr right)
            : mLeft(std::move(left))
            , mRight(std::move(right))
        {
        }

        void MoveSX::print(std::ostream& stream) const
        {
            stream << std::format("MOVE {} -> {}\n", mRight->ident(), mLeft->ident());
        }

        void MoveSX::emit(MC::Builder& builder)
        {
            instruction::OperandPtr left = mLeft->asmOperand();
            if (auto label = dynamic_cast<instruction::LabelOperand*>(left.get()))
            {
                (void)left.release();
                instruction::LabelOperandPtr lbl = instruction::LabelOperandPtr(label);
                left = std::make_unique<instruction::Relative>(std::move(lbl), std::nullopt);
            }
            builder.addValue(std::make_unique<instruction::MovSXInstruction>(std::move(left), mRight->asmOperand(), mLeft->size()));
        }
    }
}