// Copyright 2024 solar-mist

/*
 * ConstantStructs are initializer lists
 * for a specific struct type
*/

#ifndef VIPIR_IR_CONSTANT_CONSTANT_STRUCT_H
#define VIPIR_IR_CONSTANT_CONSTANT_STRUCT_H 1

#include "vipir/IR/Value.h"

#include <cstdint>

namespace vipir
{
    class BasicBlock;
    class ConstantStruct : public Value
    {
    friend class IRBuilder;
    public:
        static ConstantStruct* Get(Module& module, Type* type, std::vector<Value*> values);

        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;

        bool isConstant() const override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        ConstantStruct(Module& module, Type* type, std::vector<Value*> values);

        std::vector<Value*> mValues;
    };
}

#endif // VIPIR_IR_CONSTANT_CONSTANT_STRUCT_H