// Copyright 2025 solar-mist


#ifndef VIPIR_DI_DI_VARIABLE_H
#define VIPIR_DI_DI_VARIABLE_H 1

#include <string>

namespace vipir
{
    class Function;
    class Value;
    class DIBuilder;
    class DIType;

    class DIVariable
    {
    friend class DIBuilder;
        DIVariable(std::string name, Function* parent, Value* value, DIType* type, int line, int col);

        std::string mName;
        Function* mParent;
        Value* mValue;
        DIType* mType;
        int mLine;
        int mCol;
    };
}

#endif // VIPIR_DI_DI_VARIABLE_H