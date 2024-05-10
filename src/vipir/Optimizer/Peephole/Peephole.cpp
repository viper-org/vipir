// Copyright 2024 solar-mist


#include "vipir/Optimizer/Peephole/Peephole.h"

#include "vipir/LIR/Instruction/Move.h"
#include "vipir/LIR/Instruction/LoadAddress.h"

#include <algorithm>

namespace vipir
{
    namespace opt
    {
        void Peephole::doOptimizations(lir::Builder& builder)
        {
            bool hasOptimized = false;
            auto& instructions = builder.getValues();

            for (auto it = instructions.begin(); it != instructions.end();)
            {
                bool iterate = true;
                if (checkMoveEqualArgs(*it))
                {
                    it = eraseValue(instructions, it);
                    iterate = false;
                    hasOptimized = true;
                }
                if ((it + 1) != instructions.end())
                {
                    auto leaMoveResult = checkLeaMove(*it, *(it + 1));
                    if (checkDoubleMove(*it, *(it + 1)))
                    {
                        it = eraseValue(instructions, it);
                        it = eraseValue(instructions, it);
                        iterate = false;
                        hasOptimized = true;
                    }
                    else if (leaMoveResult != LeaMoveResult::None)
                    {
                        auto leaFirst = dynamic_cast<lir::LoadAddress*>(it->get());
                        auto moveSecond = dynamic_cast<lir::Move*>((it + 1)->get());
                        lir::OperandPtr left = moveSecond->mLeft->clone();
                        lir::OperandPtr right = leaFirst->mRight->clone();

                        it = eraseValue(instructions, it);
                        it = eraseValue(instructions, it);
                        if (leaMoveResult == LeaMoveResult::Indirect)
                            it = instructions.insert(it, std::make_unique<lir::Move>(std::move(left), std::move(right)));
                        else // direct
                            it = instructions.insert(it, std::make_unique<lir::LoadAddress>(std::move(left), std::move(right)));
                        iterate = false;
                        hasOptimized = true;
                    }
                }
                if (checkDeadStore(*it, instructions, it - instructions.begin()))
                {
                    it = eraseValue(instructions, it);
                    iterate = false;
                    hasOptimized = true;
                }
                if (iterate) ++it;
            }

            if (hasOptimized) doOptimizations(builder); // Repeat until no more optimizations are available
        }

        std::vector<lir::ValuePtr>::iterator Peephole::eraseValue(std::vector<lir::ValuePtr>& values, std::vector<lir::ValuePtr>::iterator it)
        {
            auto& value = *it;

            for (auto& input : value->getInputOperands())
            {
                if (auto vreg = dynamic_cast<lir::VirtualReg*>(input.get().get()))
                {
                    vreg->mVreg->mUses--;
                }
            }

            for (auto& output : value->getOutputOperands())
            {
                if (auto vreg = dynamic_cast<lir::VirtualReg*>(output.get().get()))
                {
                    vreg->mVreg->mUses--;
                }
            }

            return values.erase(it);
        }



        /*
         * Check for consecutive pairs of instructions that move values between
         * each other twice. For example:
         * MOV R1, R2
         * MOV R2, R1
         *
         * This can be simplified to MOV R1, R2 as R2 already contains the desired
         * value, so no move from R1 is required
         */
        bool Peephole::checkDoubleMove(lir::ValuePtr& first, lir::ValuePtr& second)
        {
            auto moveFirst = dynamic_cast<lir::Move*>(first.get());
            if (!moveFirst) return false;
            auto moveSecond = dynamic_cast<lir::Move*>(second.get());
            if (!moveSecond) return false;

            if (*moveFirst->mLeft == moveSecond->mRight)
            {
                if (*moveFirst->mRight == moveSecond->mLeft)
                {
                    return true;
                }
            }
            return false;
        }


        /*
         * Check for a mov where the operands are identical, and remove
         * them. For example:
         *
         * MOV R1, R1
         *
         * This instruction can be removed in its entirety
         */
        bool Peephole::checkMoveEqualArgs(lir::ValuePtr& value)
        {
            auto move = dynamic_cast<lir::Move*>(value.get());
            if (!move) return false;

            return (*move->mLeft == move->mRight);
        }


        /*
         * Check for a lea into a register, then an indirect move from that register
         * immediately after. For example:
         * LEA R1, [MEM]
         * MOV R2, [R1]
         *
         * This can be simplified to MOV R2, [MEM]
         *
         * Also checks for a lea into a register, then a direct move from that
         * register, as this can be simplified to a lea directly into the target
         */
        Peephole::LeaMoveResult Peephole::checkLeaMove(lir::ValuePtr& first, lir::ValuePtr& second)
        {
            auto leaFirst = dynamic_cast<lir::LoadAddress*>(first.get());
            if (!leaFirst) return LeaMoveResult::None;
            auto moveSecond = dynamic_cast<lir::Move*>(second.get());
            if (!moveSecond) return LeaMoveResult::None;

            if (*moveSecond->mRight == leaFirst->mLeft) return LeaMoveResult::Direct;

            if (auto mem = dynamic_cast<lir::Memory*>(moveSecond->mRight.get()))
            {
                if (!mem->mDisplacement && !mem->mIndex && !mem->mScale)
                {
                    if (*leaFirst->mLeft == mem->mBase)
                    {
                        return LeaMoveResult::Indirect;
                    }
                }
            }
            return LeaMoveResult::None;
        }


        /*
         * Check for a store into a physical/virtual register that is not followed by a move
         * out of that register until after a new value has been stored into it. For example:
         * MOV R1, [MEM]
         * MOV R2, [MEM2]
         * MOV R3, R2
         * The first MOV can be removed as it is a dead store
         */
        bool Peephole::checkDeadStore(lir::ValuePtr& value, std::vector<lir::ValuePtr>& values, int index)
        {
            auto move = dynamic_cast<lir::Move*>(value.get());
            if (!move) return false;

            auto& operand = move->mLeft;
            if (!dynamic_cast<lir::VirtualReg*>(operand.get())) return false;
            for (int i = index+1; i < values.size(); ++i)
            {
                auto inputs = values[i]->getInputOperands();
                auto outputs = values[i]->getOutputOperands();

                if (std::find_if(inputs.begin(), inputs.end(), [&operand](const auto input){
                    return *operand == input;
                }) != inputs.end()) return false;

                if (std::find_if(outputs.begin(), outputs.end(), [&operand](const auto output){
                    return *operand == output;
                }) != outputs.end()) return false;
            }
            return true; // Value is never used again
        }
    }
}