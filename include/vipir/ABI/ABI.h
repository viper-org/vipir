// Copyright 2024 solar-mist


#include <vector>
#ifndef VIPIR_ABI_ABI_H
#define VIPIR_ABI_ABI_H 1

namespace vipir
{
    namespace abi
    {
        class ABI
        {
        public:
            virtual ~ABI() { }

            virtual int getReturnRegister() const = 0;

            virtual int getParameterRegister(int index) const = 0;
            virtual int getParameterRegisterCount() const = 0;

            virtual int getStackOffsetRegister() const = 0;
            virtual int getStackArgumentRegister() const = 0;

            virtual std::vector<int> getGeneralPurposeRegisters() const = 0;
            virtual std::vector<int> getCallerSavedRegisters() const = 0;
            virtual std::vector<int> getCalleeSavedRegisters() const = 0;
        };
    }
}

#endif // VIPIR_ABI_ABI_H