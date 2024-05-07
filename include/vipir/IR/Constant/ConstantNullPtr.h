// Copyright 2024 solar-mist

/*
 * ConstantNullPtr is a value that represents
 * a pointer with no value
*/

#ifndef VIPIR_IR_CONSTANT_CONSTANT_NULL_PTR_H
#define VIPIR_IR_CONSTANT_CONSTANT_NULL_PTR_H 1

#include "vipir/IR/Value.h"

#include <cstdint>

namespace vipir
{
    class BasicBlock;
    class ConstantNullPtr : public Value
    {
    friend class IRBuilder;
    public:
        static ConstantNullPtr* Get(Module& module, Type* type);

        void print(std::ostream& stream) override;
        std::string ident() const override;

        bool isConstant() const override;

    protected:
        void emit(MC::Builder& builder) override;
        void emit2(lir::Builder& builder) override;

    private:
        ConstantNullPtr(Module& module, Type* type);
    };
}

#endif // VIPIR_IR_CONSTANT_CONSTANT_NULL_PTR_H