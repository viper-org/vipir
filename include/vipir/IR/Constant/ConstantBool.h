// Copyright 2024 solar-mist

/*
 * ConstantBools are boolean literals, true
 * or false
*/

#ifndef VIPIR_IR_CONSTANT_CONSTANT_BOOL_H
#define VIPIR_IR_CONSTANT_CONSTANT_BOOL_H 1

#include "vipir/IR/Value.h"

#include <cstdint>

namespace vipir
{
    class BasicBlock;
    class ConstantBool : public Value
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;

        bool isConstant() const override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        ConstantBool(BasicBlock* parent, bool value);

        bool mValue;
    };
}

#endif // VIPIR_IR_CONSTANT_CONSTANT_BOOL_H