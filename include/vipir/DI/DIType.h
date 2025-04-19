// Copyright 2025 solar-mist

#ifndef VIPIR_DI_DI_TYPE_H
#define VIPIR_DI_DI_TYPE_H 1

#include "vipir/Type/Type.h"
#include "vipir/Type/StructType.h"

#include <string>
#include <cstdint>

namespace vipir
{
    class DIType
    {
    friend class DIBuilder;
    friend class DIStructureType;
    public:
        virtual ~DIType() = default;

    protected:
        DIType(int align) : mAlign(align) {}

        int mAlign;
        int mOffset{ -1 };
    };

    class DIBasicType : public DIType
    {
    friend class DIBuilder;

        DIBasicType(std::string name, Type* type, uint8_t encoding);

        std::string mName;
        Type* mType;
        uint8_t mEncoding;
    };

    class DIPointerType : public DIType
    {
    friend class DIBuilder;

        DIPointerType(DIType* baseType /*, uint8_t byteSize*/);

        DIType* mBaseType;
    };

    struct DIStructureTypeMember
    {
        std::string name;
        int line;
        int col;
        DIType* type;
        int location;
    };

    class DIStructureType : public DIType
    {
    friend class DIBuilder;
    public:
        void addMember(std::string name, DIType* type, int line, int col);

    private:
        DIStructureType(std::string name, StructType* structType, int line, int col);

        std::string mName;
        int mLine;
        int mCol;
        StructType* mStructType;

        std::vector<DIStructureTypeMember> mMembers;
    };
}

#endif // VIPIR_DI_DI_TYPE_H