// Copyright 2023 solar-mist

#ifndef VIPIR_IR_ARGUMENT_H
#define VIPIR_IR_ARGUMENT_H 1

#include "vipir/IR/Value.h"

namespace vipir
{
    class Function;

    class Argument : public Value
    {
    friend class Function;
    public:
        void print(std::ostream& stream) const override;
        std::string ident() const override;
        
        bool requiresRegister() const override;
        std::vector<ValueId> getOperands() override;

    protected:
        void emit(std::vector<instruction::ValuePtr>& values) final;

    private:
        Argument(Module& module, ValueId id, Type* type, std::string name);

        std::string mName;
    };
    using ArgumentPtr = std::unique_ptr<Argument>;
}

#endif