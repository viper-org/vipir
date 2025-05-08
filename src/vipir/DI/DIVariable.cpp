// Copyright 2025 solar-mist


#include "vipir/DI/DIVariable.h"

#include <cassert>

namespace vipir
{
    DIVariable::DIVariable(std::string name, Function* parent, DIType* type, int line, int col)
        : mName(std::move(name))
        , mParent(parent)
        , mType(type)
        , mLine(line)
        , mCol(col)
        , mParameter(false)
    {
    }

    DIVariable::DIVariable(std::string name, Function* parent, DIType* type)
        : mName(std::move(name))
        , mParent(parent)
        , mType(type)
        , mParameter(true)
    {
    }

    void DIVariable::addValue(Value* value, QueryAddress* start, QueryAddress* end)
    {
        mValues.push_back({start, value, nullptr, end});
    }

    void DIVariable::addPointer(DIVariable* pointee, QueryAddress* start, QueryAddress* end)
    {
        mValues.push_back({start, nullptr, pointee, end});
    }
}