// Copyright 2024 solar-mist

/*
 * ConstantArrays are initializer lists
 * for an array type
*/

#ifndef VIPIR_IR_CONSTANT_CONSTANT_ARRAY_H
#define VIPIR_IR_CONSTANT_CONSTANT_ARRAY_H 1

#include "vipir/IR/Value.h"

#include <cstdint>

namespace vipir
{
    class BasicBlock;
    class ConstantArray : public Value
    {
    friend class IRBuilder;
    public:
        static ConstantArray* Get(Module& module, Type* type, std::vector<Value*> values);

        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;

        bool isConstant() const override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        ConstantArray(Module& module, Type* type, std::vector<Value*> values);

        std::vector<Value*> mValues;
    };
}

#endif // VIPIR_IR_CONSTANT_CONSTANT_ARRAY_H