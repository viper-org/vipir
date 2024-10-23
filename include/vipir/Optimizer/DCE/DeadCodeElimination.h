// Copyright 2024 solar-mist

#ifndef VIPIR_OPTIMIZER_DCE_DEAD_CODE_ELIMINATION_H
#define VIPIR_OPTIMIZER_DCE_DEAD_CODE_ELIMINATION_H 1

#include "vipir/Pass/Pass.h"

namespace vipir
{
    class Function;
    class Value;

    namespace opt
    {
        class DeadCodeEliminator
        {
        public:
            void eliminateDeadCode(Function* function);

        private:
            void markCriticalInstructions(Function* function);
            void sweepInstructions(Function* function);

            void markValueTree(Value* value);

            bool eliminateUnreachableBlocks(Function* function);
        };

        class DCEPass : public Pass
        {
        public:
            DCEPass();

            virtual void execute(Module& module) override;
        };
    }
}

#endif // VIPIR_OPTIMIZER_DCE_DEAD_CODE_ELIMINATION_H