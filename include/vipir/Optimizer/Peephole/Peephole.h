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
        };
    }
}

#endif // VIPIR_OPTIMIZER_PEEPHOLE_PEEPHOLE_H