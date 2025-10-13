// Copyright 2025 solar-mist


#ifndef VIPIR_ABI_WINABI_H
#define VIPIR_ABI_WINABI_H 1

#include "vipir/ABI/ABI.h"

namespace vipir
{
    namespace abi
    {
        class FastCall : public CallingConvention
        {
        public:
            int getReturnRegister() const override;

            int getParameterRegister(int index) const override;
            int getParameterRegisterCount() const override;
            bool reserveRegisterParameterStack() const override { return true; }

            std::vector<int> getCallerSavedRegisters() const override;
            std::vector<int> getCalleeSavedRegisters() const override;

            int getMaxStructSize() const override;

            ArgumentPassingOrder getArgumentPassingOrder() const override;
            StackCleaner getStackCleaner() const override;

            std::string decorateName(std::string_view name, FunctionType* type) const override;
        };

        class WinABI : public ABI
        {
        public:
            const CallingConvention* getDefaultCallingConvention() const override;

            int getStackOffsetRegister() const override;
            int getStackArgumentRegister() const override;
            int getStackAlign() const override;

            std::vector<int> getGeneralPurposeRegisters() const override;

        private:
            FastCall mCallingConvention;
        };
    }
}

#endif // VIPIR_ABI_WINABI_H