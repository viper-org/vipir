// Copyright 2024 solar-mist


#ifndef VIPIR_ABI_ABI_H
#define VIPIR_ABI_ABI_H 1

#include "vipir/ABI/CallingConvention.h"

#include <vector>

namespace vipir
{
    namespace abi
    {
        class ABI
        {
        public:
            virtual ~ABI() { }

            virtual const CallingConvention* getDefaultCallingConvention() const = 0;

            virtual int getStackOffsetRegister() const = 0;
            virtual int getStackArgumentRegister() const = 0;

            virtual std::vector<int> getGeneralPurposeRegisters() const = 0;
        };
    }
}

#endif // VIPIR_ABI_ABI_H