// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Move.h"

#include "vasm/instruction/singleOperandInstruction/SetccInstruction.h"
#include "vipir/MC/CmpOperand.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/instruction/singleOperandInstruction/PushInstruction.h"

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
            if (auto cmp = dynamic_cast<CmpOperand*>(right.get()))
            {
                auto createSetcc = [&builder, &left]<typename InstT>(){
                    builder.addValue(std::make_unique<InstT>(std::move(left)));
                };
                switch(cmp->getOperator())
                {
                    case CmpOperator::EQ:
                        createSetcc.template operator()<instruction::SeteInstruction>();
                        break;
                    case CmpOperator::NE:
                        createSetcc.template operator()<instruction::SetneInstruction>();
                        break;
                    case CmpOperator::LT:
                        createSetcc.template operator()<instruction::SetlInstruction>();
                        break;
                    case CmpOperator::GT:
                        createSetcc.template operator()<instruction::SetgInstruction>();
                        break;
                    case CmpOperator::LE:
                        createSetcc.template operator()<instruction::SetleInstruction>();
                        break;
                    case CmpOperator::GE:
                        createSetcc.template operator()<instruction::SetgeInstruction>();
                        break;
                }
                return;
            }
            if (dynamic_cast<instruction::Memory*>(left.get()) && dynamic_cast<instruction::Memory*>(right.get()))
            {
                instruction::OperandPtr reg = std::make_unique<instruction::Register>(11, mRight->size());
                builder.addValue(std::make_unique<instruction::MovInstruction>(reg->clone(), std::move(right)));
                right = std::move(reg);
            }
            builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(left), std::move(right), mLeft->size()));
        }

        std::vector<std::reference_wrapper<OperandPtr> > Move::getOutputOperands()
        {
            return {mLeft};
        }

        std::vector<std::reference_wrapper<OperandPtr> > Move::getInputOperands()
        {
            return {mLeft, mRight};
        }


        MoveSX::MoveSX(OperandPtr left, OperandPtr right)
            : mLeft(std::move(left))
            , mRight(std::move(right))
        {
        }

        void MoveSX::print(std::ostream& stream) const
        {
            stream << std::format("MOVESX {} -> {}\n", mRight->ident(), mLeft->ident());
        }

        void MoveSX::emit(MC::Builder& builder)
        {
            instruction::OperandPtr left = mLeft->asmOperand();
            instruction::OperandPtr right = mRight->asmOperand();
            if (auto label = dynamic_cast<instruction::LabelOperand*>(left.get()))
            {
                (void)left.release();
                instruction::LabelOperandPtr lbl = instruction::LabelOperandPtr(label);
                left = std::make_unique<instruction::Relative>(std::move(lbl), std::nullopt);
            }

            if (dynamic_cast<instruction::Memory*>(left.get()))
            {
                instruction::OperandPtr reg = std::make_unique<instruction::Register>(0, mLeft->size());
                left = std::move(reg);
            }

            builder.addValue(std::make_unique<instruction::MovSXInstruction>(left->clone(), std::move(right), mLeft->size()));

            if (dynamic_cast<instruction::Memory*>(mLeft->asmOperand().get()))
            {
                builder.addValue(std::make_unique<instruction::MovInstruction>(mLeft->asmOperand(), std::move(left), mLeft->size()));
            }
        }

        std::vector<std::reference_wrapper<OperandPtr> > MoveSX::getOutputOperands()
        {
            return {mLeft};
        }

        std::vector<std::reference_wrapper<OperandPtr> > MoveSX::getInputOperands()
        {
            return {mRight};
        }


        MoveZX::MoveZX(OperandPtr left, OperandPtr right)
            : mLeft(std::move(left))
            , mRight(std::move(right))
        {
        }

        void MoveZX::print(std::ostream& stream) const
        {
            stream << std::format("MOVEZX {} -> {}\n", mRight->ident(), mLeft->ident());
        }

        void MoveZX::emit(MC::Builder& builder)
        {
            instruction::OperandPtr left = mLeft->asmOperand();
            if (auto label = dynamic_cast<instruction::LabelOperand*>(left.get()))
            {
                (void)left.release();
                instruction::LabelOperandPtr lbl = instruction::LabelOperandPtr(label);
                left = std::make_unique<instruction::Relative>(std::move(lbl), std::nullopt);
            }

            if (mLeft->size() == codegen::OperandSize::Quad && mRight->size() == codegen::OperandSize::Long)
            {
                if (*mLeft == mRight) return;

                builder.addValue(std::make_unique<instruction::MovInstruction>(std::move(left), mRight->asmOperand(), mRight->size()));
            }

            builder.addValue(std::make_unique<instruction::MovZXInstruction>(std::move(left), mRight->asmOperand(), mLeft->size()));
        }

        std::vector<std::reference_wrapper<OperandPtr> > MoveZX::getOutputOperands()
        {
            return {mLeft};
        }

        std::vector<std::reference_wrapper<OperandPtr> > MoveZX::getInputOperands()
        {
            return {mRight};
        }



        Push::Push(OperandPtr operand)
            : mOperand(std::move(operand))
        {
        }

        void Push::print(std::ostream& stream) const
        {
            stream << std::format("PUSH {}\n", mOperand->ident());
        }

        void Push::emit(MC::Builder& builder)
        {
            builder.addValue(std::make_unique<instruction::PushInstruction>(mOperand->asmOperand(), mOperand->size()));
        }

        std::vector<std::reference_wrapper<OperandPtr> > Push::getInputOperands()
        {
            return {mOperand};
        }
    }
}