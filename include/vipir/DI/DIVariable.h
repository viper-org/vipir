// Copyright 2025 solar-mist


#ifndef VIPIR_DI_DI_VARIABLE_H
#define VIPIR_DI_DI_VARIABLE_H 1

#include <string>
#include <vector>

namespace vipir
{
    class Function;
    class Value;
    class QueryAddress;
    class DIBuilder;
    class DIType;
    class DIVariable;

    struct ScopedValue
    {
        QueryAddress* startAddress;
        Value* value;
        DIVariable* pointee;
        QueryAddress* endAddress;
    };

    class DIVariable
    {
    friend class DIBuilder;
    public:
        void addValue(Value* value, QueryAddress* start, QueryAddress* end);
        void addPointer(DIVariable* pointee, QueryAddress* start, QueryAddress* end);

    private:
        DIVariable(std::string name, Function* parent, DIType* type, int line, int col);


        std::string mName;
        Function* mParent;
        std::vector<ScopedValue> mValues;
        DIType* mType;
        int mLine;
        int mCol;


        int mOffset{ -1 };
    };
}

#endif // VIPIR_DI_DI_VARIABLE_H