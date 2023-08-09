// Copyright 2023 solar-mist

/*
 * Function declaration or definition
*/

#ifndef MIDAS_IR_FUNCTION_H
#define MIDAS_IR_FUNCTION_H 1

#include "vipir/IR/Global.h"

#include <string>

namespace vipir
{
    class Function : public Global
    {
    public:
        static Function* Create(Module& module, std::string name);

        Module& getModule() const;
        std::string_view getName() const;

        void print(std::ostream& stream) const override;
        std::string ident() const override;
    private:
        Function(Module& module, std::string name);

        std::string mName;
    };
}

#endif // MIDAS_IR_FUNCTION_H