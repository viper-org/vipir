// Copyright 2023 solar-mist

/*
 * ConstantInts are integer literals
*/

#ifndef VIPIR_IR_CONSTANT_CONSTANT_INT_H
#define VIPIR_IR_CONSTANT_CONSTANT_INT_H 1

#include "vipir/IR/Constant/Constant.h"

#include <cstdint>

namespace vipir
{
    class Builder;
    class BasicBlock;
    class ConstantInt : public Constant
    {
    friend class Builder;
    public:
        uint64_t getValue() const;

        void print(std::ostream& stream) const override;
        std::string ident() const override;
        
        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) final;

    private:
        ConstantInt(BasicBlock* parent, ValueId id, uint64_t value, Type* type, std::string name);

        uint64_t mValue;
        std::string mName;
    };
}

#endif // VIPIR_IR_CONSTANT_CONSTANT_INT_H