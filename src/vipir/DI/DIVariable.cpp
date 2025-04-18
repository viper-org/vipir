// Copyright 2025 solar-mist


#include "vipir/DI/DIVariable.h"

namespace vipir
{
    DIVariable::DIVariable(std::string name, Function* parent, Value* value, DIType* type, int line, int col)
        : mName(std::move(name))
        , mParent(parent)
        , mValue(value)
        , mType(type)
        , mLine(line)
        , mCol(col)
    {
    }
}