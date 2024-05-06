// Copyright 2024 solar-mist


#include "vipir/Optimizer/Peephole/Peephole.h"

#include "vipir/LIR/Instruction/Move.h"

namespace vipir
{
    namespace opt
    {
        void Peephole::doOptimizations(lir::Builder& builder)
        {
            auto& instructions = builder.getValues();

            for (auto it = instructions.begin(); it != instructions.end();)
            {
                if ((it + 1) != instructions.end())
                {
                    if (checkDoubleMove(*it, *(it + 1)))
                    {
                        it = instructions.erase(it + 1);
                    }
                }
                ++it;
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
    }
}