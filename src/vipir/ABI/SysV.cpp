// Copyright 2024 solar-mist


#include "vipir/ABI/SysV.h"

#include <array>

namespace vipir
{
    namespace abi
    {
        int SysV::getReturnRegister() const
        {
            return 0; // rax
        }

        int SysV::getParameterRegister(int index) const
        {
            constexpr std::array registers {7, 6, 1, 2};

            return registers[index];
        }

        int SysV::getStackOffsetRegister() const
        {
            return 5; // rbp
        }

        std::vector<int> SysV::getGeneralPurposeRegisters() const
        {
            return { 1, 2, 3, 6, 7 };
        }

        std::vector<int> SysV::getCallerSavedRegisters() const
        {
            return { 0, 1, 2, 6, 7 };
        }

        std::vector<int> SysV::getCalleeSavedRegisters() const
        {
            return { 3 };
        }
    }
}