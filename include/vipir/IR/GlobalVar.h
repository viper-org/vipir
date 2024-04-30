// Copyright 2024 solar-mist

/*
 * GlobalVars are a value in memory
 * that exist in the global scope
*/

#ifndef VIPIR_IR_GLOBAL_VAR_H
#define VIPIR_IR_GLOBAL_VAR_H 1

#include "vipir/IR/Global.h"

namespace vipir
{
    class GlobalVar : public Global
    {
    friend class Module;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;

        void setInitialValue(Value* value);

    protected:
        void emit(MC::Builder& builder) override;

    private:
        GlobalVar(Module& module, Type* type);

        std::unique_ptr<Value> mInitialValue;
        int mValueId;
    };
}

#endif // VIPIR_IR_GLOBAL_VAR_H