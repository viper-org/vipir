// Copyright 2024 solar-mist

#ifndef VIPIR_OPTIMIZER_MEM2REG_DOM_H
#define VIPIR_OPTIMIZER_MEM2REG_DOM_H 1

#include "vipir/Pass/Pass.h"

namespace vipir
{
    class Function;
    class BasicBlock;
    class Value;

    namespace opt
    {
        class DominatorAnalyzer
        {
        public:
            void computeDominanceFrontiers(Function* function);

        private:
            BasicBlock* Intersect(BasicBlock* b1, BasicBlock* b2);
        };
    }
}

#endif // VIPIR_OPTIMIZER_MEM2REG_DOM_H