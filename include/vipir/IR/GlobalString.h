// Copyright 2024 solar-mist

/*
 * GlobalStrings are pointers to a string literal
 * that can be accessed anywhere in the program
*/

#ifndef VIPIR_IR_GLOBAL_STRING_H
#define VIPIR_IR_GLOBAL_STRING_H 1

#include "vipir/IR/Global.h"

namespace vipir
{
    class GlobalString : public Global
    {
    public:
        static GlobalString* Create(Module& module, std::string value);

        void print(std::ostream& stream) override;
        std::string ident() const override;

    protected:
        void emit(MC::Builder& builder) override;

    private:
        GlobalString(Module& module, std::string value);

        std::string mValue;
        int mValueId;
    };
}

#endif // VIPIR_IR_GLOBAL_STRING_H