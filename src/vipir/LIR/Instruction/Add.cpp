// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Add.h"

#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        Add::Add(OperandPtr dest, OperandPtr left, OperandPtr right)
            : mDest(std::move(dest))
            , mLeft(std::move(left))
            , mRight(std::move(right))
        {
        }

        void Add::print(std::ostream& stream) const
        {
            stream << std::format("ADD {} + {} -> {}\n", mLeft->ident(), mRight->ident(), mDest->ident());
        }

        void Add::emit(MC::Builder& builder)
        {
            if (*mDest != mLeft)
            {
                builder.addValue(std::make_unique<instruction::MovInstruction>(mDest->asmOperand(), mLeft->asmOperand()));
            }

            builder.addValue(std::make_unique<instruction::AddInstruction>(mDest->asmOperand(), mRight->asmOperand()));
        }
    }
}