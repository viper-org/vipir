// Copyright 2024 solar-mist

#ifndef VIPIR_OPTIMIZER_PEEPHOLE_PEEPHOLE_H
#define VIPIR_OPTIMIZER_PEEPHOLE_PEEPHOLE_H 1

#include "vipir/LIR/Builder.h"

namespace vipir
{
    namespace opt
    {
        class Peephole
        {
        public:
            void doOptimizations(lir::Builder& builder);
        
        private:
            bool checkDoubleMove(lir::ValuePtr& first, lir::ValuePtr& second);
            enum class LeaMoveResult
            {
                None,
                Direct,
                Indirect
            };
            LeaMoveResult checkLeaMove(lir::ValuePtr& first, lir::ValuePtr& second);
            bool checkDeadStore(lir::ValuePtr& value, std::vector<lir::ValuePtr>& values, int index);
        };
    }
}

#endif // VIPIR_OPTIMIZER_PEEPHOLE_PEEPHOLE_H