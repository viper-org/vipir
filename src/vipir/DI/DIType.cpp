// Copyright 2025 solar-mist

#include "vipir/DI/DIType.h"

namespace vipir
{
    DIBasicType::DIBasicType(std::string name, Type* type, uint8_t encoding)
        : DIType(type->getAlignment())
        , mName(std::move(name))
        , mType(type)
        , mEncoding(encoding)
    {
    }


    DIPointerType::DIPointerType(DIType* baseType)
        : DIType(8)
        , mBaseType(baseType)
    {
    }


    void DIStructureType::addMember(std::string name, DIType* type, int line, int col)
    {
        mMembers.push_back({std::move(name), line, col, type, -1});
    }

    DIStructureType::DIStructureType(std::string name, StructType* structType, int line, int col)
        : DIType(0)
        , mName(std::move(name))
        , mLine(line)
        , mCol(col)
        , mStructType(structType)
    {
    }
}