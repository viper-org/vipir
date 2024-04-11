// Copyright 2024 solar-mist

/*
 * Function IR nodes represent a function
 * definition or an extern function declaration
*/

#ifndef VIPIR_IR_FUNCTION_H
#define VIPIR_IR_FUNCTION_H 1

#include "vipir/IR/Global.h"

namespace vipir
{
    class Module;
    
    class Function : public Global
    {
    public:
        static Function* Create(/*FunctionType* type, */Module& module, std::string_view name);

        void print(std::ostream& stream) override;

    protected:
        void emit(MC::Builder& builder) override;

    private:
        Function(/*FunctionType* type, */Module& module, std::string_view name);

        std::string mName;
    };
}

#endif // VIPIR_IR_FUNCTION_H