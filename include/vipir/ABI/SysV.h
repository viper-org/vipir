// Copyright 2024 solar-mist


#ifndef VIPIR_ABI_SYSV_H
#define VIPIR_ABI_SYSV_H 1

#include "vipir/ABI/ABI.h"

namespace vipir
{
    namespace abi
    {
        class SysV : public ABI
        {
        public:
            int getReturnRegister() const override;
            int getParameterRegister(int index) const override; 
        };
    }
}

#endif // VIPIR_ABI_SYSV_H