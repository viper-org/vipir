// Copyright 2024 solar-mist


#include "vipir/Optimizer/Peephole/Peephole.h"

#include "vipir/LIR/Instruction/Move.h"
#include "vipir/LIR/Instruction/LoadAddress.h"

namespace vipir
{
    namespace opt
    {
        void Peephole::doOptimizations(lir::Builder& builder)
        {
            auto& instructions = builder.getValues();

            int index = 0;
            for (auto it = instructions.begin(); it != instructions.end();)
            {
                bool iterate = true;
                if ((it + 1) != instructions.end())
                {
                    if (checkDoubleMove(*it, *(it + 1)))
                    {
                        it = instructions.erase(it + 1);
                        iterate = false;
                    }
                    else if (checkLeaMove(*it, *(it + 1)))
                    {
                        auto leaFirst = dynamic_cast<lir::LoadAddress*>(it->get());
                        auto moveSecond = dynamic_cast<lir::Move*>((it + 1)->get());
                        lir::OperandPtr left = moveSecond->mLeft->clone();
                        lir::OperandPtr right = leaFirst->mRight->clone();

                        it = instructions.erase(it, it + 2);
                        it = instructions.insert(it, std::make_unique<lir::Move>(std::move(left), std::move(right)));
                        iterate = false;
                    }
                }
                if (checkDeadStore(*it, instructions, index))
                {
                    it = instructions.erase(it);
                }
                if (iterate) ++it;

                ++index;
            }
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
         * Check for a lea into a register, then an indirect move from that register
         * immediately after. For example:
         * LEA R1, [MEM]
         * MOV R2, [R1]
         *
         * This can be simplified to MOV R2, [MEM]
         */
        bool Peephole::checkLeaMove(lir::ValuePtr& first, lir::ValuePtr& second)
        {
            auto leaFirst = dynamic_cast<lir::LoadAddress*>(first.get());
            if (!leaFirst) return false;
            auto moveSecond = dynamic_cast<lir::Move*>(second.get());
            if (!moveSecond) return false;

            if (auto mem = dynamic_cast<lir::Memory*>(moveSecond->mRight.get()))
            {
                if (!mem->mDisplacement && !mem->mIndex && !mem->mScale)
                {
                    if (*leaFirst->mLeft == mem->mBase)
                    {
                        return true;
                    }
                }
            }
            return false;
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
            for (int i = index; i < values.size(); ++i)
            {
                if (auto move = dynamic_cast<lir::Move*>(values[i].get()))
                {
                    if (*move->mLeft == operand) return true;
                    else if (*move->mRight == operand) return false;
                }
            }
            return false;
        }
    }
}