// Copyright 2024 solar-mist

/*
 * ConstantInts are integer literals
*/

#ifndef VIPIR_IR_CONSTANT_CONSTANT_INT_H
#define VIPIR_IR_CONSTANT_CONSTANT_INT_H 1

#include "vipir/IR/Value.h"

#include <cstdint>

namespace vipir
{
    class BasicBlock;
    class ConstantInt : public Value
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;

        intmax_t getValue() const;

    protected:
        void emit(MC::Builder& builder) override;

    private:
        ConstantInt(BasicBlock* parent, intmax_t value, Type* type);

        intmax_t mValue;
    };
}

#endif // VIPIR_IR_CONSTANT_CONSTANT_INT_H