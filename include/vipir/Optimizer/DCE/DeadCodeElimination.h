// Copyright 2024 solar-mist

#ifndef VIPIR_OPTIMIZER_DCE_DEAD_CODE_ELIMINATION_H
#define VIPIR_OPTIMIZER_DCE_DEAD_CODE_ELIMINATION_H 1

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

            bool eliminateUnreachableBlocks(Function* function);
        };
    }
}

#endif // VIPIR_OPTIMIZER_DCE_DEAD_CODE_ELIMINATION_H