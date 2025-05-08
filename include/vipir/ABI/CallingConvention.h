// Copyright 2025 JesusTouchMe

#ifndef VIPIR_ABI_CALLINGCONVENTION_H
#define VIPIR_ABI_CALLINGCONVENTION_H 1

#include "vipir/Type/FunctionType.h"

#include <vector>
#include <string>

namespace vipir
{
    namespace abi
    {
        enum class ArgumentPassingOrder
        {
            LeftToRight,
            RightToLeft,
        };

        enum class StackCleaner
        {
            Caller,
            Callee,
        };

        class CallingConvention
        {
        public:
            virtual ~CallingConvention() { }

            virtual int getReturnRegister() const = 0;

            virtual int getParameterRegister(int index) const = 0;
            virtual int getParameterRegisterCount() const = 0;
            
            // Maximum size of a struct that can be passed in registers
            virtual int getMaxStructSize() const = 0;

            virtual std::vector<int> getCallerSavedRegisters() const = 0;
            virtual std::vector<int> getCalleeSavedRegisters() const = 0;

            virtual ArgumentPassingOrder getArgumentPassingOrder() const = 0;
            virtual StackCleaner getStackCleaner() const = 0;

            virtual std::string decorateName(std::string_view name, FunctionType* type) const = 0;
        };
    }
}

#endif // VIPIR_ABI_CALLINGCONVENTION_H
