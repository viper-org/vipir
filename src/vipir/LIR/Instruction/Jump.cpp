// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Jump.h"

#include "vipir/MC/CmpOperand.h"

#include "vasm/instruction/singleOperandInstruction/JmpInstruction.h"
#include "vasm/instruction/singleOperandInstruction/JccInstruction.h"
#include "vasm/instruction/singleOperandInstruction/CallInstruction.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        Jump::Jump(OperandPtr dest)
            : mDest(std::move(dest))
        {
        }

        void Jump::print(std::ostream& stream) const
        {
            stream << std::format("JUMP {}\n", mDest->ident());
        }

        void Jump::emit(MC::Builder& builder)
        {
            builder.addValue(std::make_unique<instruction::JmpInstruction>(mDest->asmOperand()));
        }


        CondJump::CondJump(OperandPtr dest, OperandPtr cond)
            : mDest(std::move(dest))
            , mCondition(std::move(cond))
        {
        }

        void CondJump::print(std::ostream& stream) const
        {
            auto cmpOperand = static_cast<CMP*>(mCondition.get());
            stream << std::format("JUMP {} IF {}\n", mDest->ident(), cmpOperand->operatorName());
        }

        void CondJump::emit(MC::Builder& builder)
        {
            instruction::OperandPtr cond = mCondition->asmOperand();
            auto cmp = static_cast<CmpOperand*>(cond.get());
            switch (cmp->getOperator())
            {
                case CmpOperator::EQ:
                    builder.addValue(std::make_unique<instruction::JeInstruction>(mDest->asmOperand()));
                    break;
                case CmpOperator::NE:
                    builder.addValue(std::make_unique<instruction::JneInstruction>(mDest->asmOperand()));
                    break;
                case CmpOperator::LT:
                    builder.addValue(std::make_unique<instruction::JlInstruction>(mDest->asmOperand()));
                    break;
                case CmpOperator::GT:
                    builder.addValue(std::make_unique<instruction::JgInstruction>(mDest->asmOperand()));
                    break;
                case CmpOperator::LE:
                    builder.addValue(std::make_unique<instruction::JleInstruction>(mDest->asmOperand()));
                    break;
                case CmpOperator::GE:
                    builder.addValue(std::make_unique<instruction::JgeInstruction>(mDest->asmOperand()));
                    break;
            }
        }



        Call::Call(OperandPtr dest)
            : mDest(std::move(dest))
        {
        }

        void Call::print(std::ostream& stream) const
        {
            stream << std::format("CALL {}", mDest->ident());
        }

        void Call::emit(MC::Builder& builder)
        {
            builder.addValue(std::make_unique<instruction::CallInstruction>(mDest->asmOperand()));
        }
    }
}