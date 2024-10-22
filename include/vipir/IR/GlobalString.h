// Copyright 2024 solar-mist

/*
 * GlobalStrings represent a string value that
 * exist in the global scope
*/

#ifndef VIPIR_IR_GLOBAL_STRING_H
#define VIPIR_IR_GLOBAL_STRING_H 1

#include "vipir/IR/Global.h"

namespace vipir
{
    class GlobalString : public Global
    {
    friend class IRBuilder;
    public:
        static GlobalString* Create(Module& module, std::string value);

        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        GlobalString(Module& module, std::string value);

        std::string mValue;
        int mValueId;
    };
}

#endif // VIPIR_IR_GLOBAL_STRING_H