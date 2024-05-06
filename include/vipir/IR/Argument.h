// Copyright 2024 solar-mist

/*
 * Arguments are a value that has been passed
 * into a Function
*/

#ifndef VIPIR_IR_ARGUMENT_H
#define VIPIR_IR_ARGUMENT_H 1

#include "vipir/IR/Value.h"

namespace vipir
{
    class Argument : public Value
    {
    friend class Function;
    public:
        Argument(Module& module, Type* type, std::string&& name, int index);

        void print(std::ostream& stream) override;
        std::string ident() const override;
    protected:
        void emit(MC::Builder& builder) override;
        void emit2(lir::Builder& builder) override;

    private:
        std::string mName;
        int mIdx;
    };

    using ArgumentPtr = std::unique_ptr<Argument>;
}

#endif // VIPIR_IR_ARGUMENT_H