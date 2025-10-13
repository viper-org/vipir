// Copyright 2024 solar-mist


#include "vipir/ABI/SysV.h"

#include <array>

namespace vipir
{
    namespace abi
    {

        int SysVCall::getReturnRegister() const
        {
            return 0; // rax
        }

        int SysVCall::getParameterRegister(int index) const
        {
            constexpr std::array registers { 7, 6, 2, 1, 8, 9 };

            if (index >= registers.size()) return -1;

            return registers[index];
        }

        int SysVCall::getParameterRegisterCount() const
        {
            return 6;
        }

        std::vector<int> SysVCall::getCallerSavedRegisters() const
        {
            return { 0, 1, 2, 6, 7, 8, 9, 10, 11 };
        }

        std::vector<int> SysVCall::getCalleeSavedRegisters() const
        {
            return { 3, 12, 13, 14, 15 };
        }

        int SysVCall::getMaxStructSize() const
        {
            return 128; // 16 bytes
        }

        ArgumentPassingOrder SysVCall::getArgumentPassingOrder() const
        {
            return ArgumentPassingOrder::RightToLeft; // Parameters to functions are passed in the registers rdi, rsi, rdx, rcx, r8, r9, and further values are passed on the stack in reverse order. - some website
        }

        StackCleaner SysVCall::getStackCleaner() const
        {
            return StackCleaner::Caller;
        }

        std::string SysVCall::decorateName(std::string_view name, FunctionType* type) const
        {
            return std::string(name);
        }

        const CallingConvention* SysV::getDefaultCallingConvention() const
        {
            return &mCallingConvention;
        }

        int SysV::getStackOffsetRegister() const
        {
            return 5; // rbp
        }

        int SysV::getStackArgumentRegister() const
        {
            return 4; // rsp
        }

        int SysV::getStackAlign() const
        {
            return 0;
        }

        std::vector<int> SysV::getGeneralPurposeRegisters() const
        {
            // No '11' because this is used in case of mov mem, mem
            return { 0, 1, 2, 6, 7, 8, 9, 10, 3, 12, 13, 14, 15 };
        }
    }
}