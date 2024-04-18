// Copyright 2024 solar-mist


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
        };
    }
}

#endif // VIPIR_ABI_ABI_H