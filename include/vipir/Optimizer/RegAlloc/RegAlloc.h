// Copyright 2024 solar-mist

#ifndef VIPIR_OPTIMIZER_REGALLOC_REGALLOC_H
#define VIPIR_OPTIMIZER_REGALLOC_REGALLOC_H 1

#include "vipir/IR/Function.h"

#include "vipir/ABI/ABI.h"

#include <map>

namespace vipir
{
    namespace opt
    {
        class RegAlloc
        {
        public:
            void assignVirtualRegisters(Function* function, abi::ABI* abi);

        private:
            void setLiveIntervals(Function* function);
            void setStackOffsets(Function* function, abi::ABI* abi);

            struct ActiveValueComparator
            {
                bool operator()(Value* a, Value* b) const
                {
                    return a->mInterval.second <= b->mInterval.second;
                }
            };
        };
    }
}

#endif // VIPIR_OPTIMIZER_REGALLOC_REGALLOC_H