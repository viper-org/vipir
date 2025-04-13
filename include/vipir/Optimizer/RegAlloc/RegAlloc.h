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
            void doRegalloc(Function* function, std::map<int, VReg*>& virtualRegs, int virtualRegCount, abi::ABI* abi);

            void setLiveIntervals(Function* function);
            void setStackOffsets(Function* function, abi::ABI* abi);

            struct ActiveValueComparator
            {
                bool operator()(Value* a, Value* b) const
                {
                    return a->mInterval.second < b->mInterval.second;
                }
            };

            template<typename It>
            void addStackParams(It begin, It end, Function* function, abi::ABI* abi, std::vector<Value*>& activeValues)
            {
                int stackOffset = -0x10;

                for (auto it = begin; it < end; ++it)
                {
                    ArgumentPtr& argument = *it;

                    int id = function->mVirtualRegs.size();
                    function->mVirtualRegs.push_back(std::make_unique<VReg>(id, abi->getStackOffsetRegister()));
                    VReg* vreg = function->mVirtualRegs.back().get();

                    vreg->mOnStack = true;
                    vreg->mArgument = true;
                    vreg->mStackOffset = stackOffset;
                    stackOffset -= 8;

                    argument->mVReg = vreg;
                    activeValues.push_back(argument.get());
                }
            }

            std::vector<Argument*> setArguments(Function* function, abi::ABI* abi, std::vector<Value*>& activeValues, std::map<int, VReg*>& virtualRegs);
        };
    }
}

#endif // VIPIR_OPTIMIZER_REGALLOC_REGALLOC_H