// Copyright 2025 solar-mist


#include "vipir/ABI/WinABI.h"

#include <array>

namespace vipir
{
    namespace abi
    {
        int FastCall::getReturnRegister() const
        {
            return 0; // rax
        }

        int FastCall::getParameterRegister(int index) const
        {
            constexpr std::array registers{ 1, 2, 8, 9 };

            if (index >= registers.size()) return -1;

            return registers[index];
        }

        int FastCall::getParameterRegisterCount() const
        {
            return 4;
        }

        std::vector<int> FastCall::getCallerSavedRegisters() const
        {
            return { 0, 1, 2, 8, 9, 10, 11 };
        }

        std::vector<int> FastCall::getCalleeSavedRegisters() const
        {
            return { 3, 5, 6, 7, 12, 13, 14, 15 };
        }

        int FastCall::getMaxStructSize() const
        {
            return 128; // 16 bytes
        }

        ArgumentPassingOrder FastCall::getArgumentPassingOrder() const
        {
            return ArgumentPassingOrder::RightToLeft; // Parameters to functions are passed in the registers rdi, rsi, rdx, rcx, r8, r9, and further values are passed on the stack in reverse order. - some website
        }

        StackCleaner FastCall::getStackCleaner() const
        {
            return StackCleaner::Caller;
        }

        std::string FastCall::decorateName(std::string_view name, FunctionType* type) const
        {
            return std::string(name);
        }

        const CallingConvention* WinABI::getDefaultCallingConvention() const
        {
            return &mCallingConvention;
        }

        int WinABI::getStackOffsetRegister() const
        {
            return 5; // rbp
        }

        int WinABI::getStackArgumentRegister() const
        {
            return 4; // rsp
        }

        int WinABI::getReservedStackSize() const
        {
            return 0x28;
        }

        std::vector<int> WinABI::getGeneralPurposeRegisters() const
        {
            // No '11' because this is used in case of mov mem, mem
            return { 0, 1, 2, 6, 7, 8, 9, 10, 3, 12, 13, 14, 15 };
        }
    }
}