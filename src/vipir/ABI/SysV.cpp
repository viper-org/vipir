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
            constexpr std::array registers { 7, 6, 2, 1, 8, 9 };

            if (index >= registers.size()) return -1;

            return registers[index];
        }

        int SysV::getParameterRegisterCount() const
        {
            return 6;
        }

        int SysV::getStackOffsetRegister() const
        {
            return 5; // rbp
        }

        int SysV::getStackArgumentRegister() const
        {
            return 4; // rsp
        }

        std::vector<int> SysV::getGeneralPurposeRegisters() const
        {
            // No '11' because this is used in case of mov mem, mem
            return { 0, 1, 2, 6, 7, 8, 9, 10, 3, 12, 13, 14, 15 };
        }

        std::vector<int> SysV::getCallerSavedRegisters() const
        {
            return { 0, 1, 2, 6, 7, 8, 9, 10, 11 };
        }

        std::vector<int> SysV::getCalleeSavedRegisters() const
        {
            return { 3, 12, 13, 14, 15 };
        }
    }
}