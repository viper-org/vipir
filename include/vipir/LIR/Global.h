// Copyright 2024 solar-mist

#ifndef VIPIR_LIR_GLOBAL_H
#define VIPIR_LIR_GLOBAL_H 1

#include "vipir/LIR/Value.h"

namespace vipir
{
    namespace lir
    {
        class GlobalString : public Value
        {
        public:
            GlobalString(std::string value);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;
        
        private:
            std::string mValue;
        };

        class GlobalDecl : public Value
        {
        public:
            GlobalDecl(OperandPtr value, codegen::OperandSize size);

            void print(std::ostream& stream) const override;
            void emit(MC::Builder& builder) override;
        
        private:
            OperandPtr mValue;
            codegen::OperandSize mSize;
        };
    }
}

#endif // VIPIR_LIR_GLOBAL_H