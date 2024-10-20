// Copyright 2024 solar-mist

#ifndef VIPIR_OPTIMIZER_PEEPHOLE_PEEPHOLE_H
#define VIPIR_OPTIMIZER_PEEPHOLE_PEEPHOLE_H 1

#include "vipir/LIR/Builder.h"

namespace vipir
{
    namespace opt
    {
        class PeepholeV2
        {
        public:
            void doPeephole(lir::Builder& builder);
        
        private:
            enum class UnreachableLabelLevel
            {
                Unreachable,
                OptimizedReferences,
                References
            };

            UnreachableLabelLevel checkUnreachableLabel(lir::Builder& builder, lir::ValuePtr& label);
            bool checkJmpLabel(lir::ValuePtr& first, lir::ValuePtr& second);

            std::vector<lir::ValuePtr>::iterator eraseValue(std::vector<lir::ValuePtr>& values, std::vector<lir::ValuePtr>::iterator it);
        };
    }
}

#endif // VIPIR_OPTIMIZER_PEEPHOLE_PEEPHOLE_H