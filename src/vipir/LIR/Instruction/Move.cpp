// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Register.h"

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
            builder.addValue(std::make_unique<instruction::MovInstruction>(mLeft->asmOperand(), mRight->asmOperand(), mLeft->size()));
        }


        MoveIndirect::MoveIndirect(OperandPtr left, OperandPtr right)
            : mLeft(std::move(left))
            , mRight(std::move(right))
        {
        }

        void MoveIndirect::print(std::ostream& stream) const
        {
            stream << std::format("MOVE INDIRECT {} -> {}\n", mRight->ident(), mLeft->ident());
        }

        void MoveIndirect::emit(MC::Builder& builder)
        {
            instruction::OperandPtr right = mRight->asmOperand();
            if (mRight->isMemory())
            {
                builder.addValue(std::make_unique<instruction::MovInstruction>(mLeft->asmOperand(), mRight->asmOperand()));
                right = mLeft->asmOperand();
            }

            if (auto reg = dynamic_cast<instruction::Register*>(right.get()))
            {
                int id = reg->getID();
                codegen::OperandSize size = reg->getSize();

                instruction::RegisterPtr base = std::make_unique<instruction::Register>(id, size);

                right = std::make_unique<instruction::Memory>(std::move(base), std::nullopt, nullptr, std::nullopt);
            }

            builder.addValue(std::make_unique<instruction::MovInstruction>(mLeft->asmOperand(), std::move(right)));
        }
    }
}