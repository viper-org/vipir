// Copyright 2024 solar-mist


#include "vipir/ABI/SysV.h"

#include <array>

namespace vipir
{
    namespace abi
    {
        int SysV::getReturnRegister() const
        {
            return 0; // EAX
        }

        int SysV::getParameterRegister(int index) const
        {
            constexpr std::array registers {7, 6, 1, 2};

            return registers[index];
        }
    }
}