// Copyright 2025 solar-mist


#ifndef VIPIR_DI_DI_BUILDER_H
#define VIPIR_DI_DI_BUILDER_H 1

#include "vipir/DI/DIVariable.h"
#include "vipir/DI/DIType.h"

#include "vipir/MC/Builder.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace codegen
{
    class OpcodeBuilder;
}

namespace vipir
{
    class Module;
    class Value;
    class Type;

    struct ULEB128
    {
        explicit ULEB128(uint64_t val) : mValue(val) {}

        uint64_t mValue;
    };
    inline ULEB128 operator""_uleb(unsigned long long val) { return ULEB128(val); };

    struct SLEB128
    {
        explicit SLEB128(int64_t val) : mValue(val) {}

        int64_t mValue;
    };

    struct TypeOffset
    {
        explicit TypeOffset(DIType* type) : mType(type) {}

        DIType* mType;
    };

    struct Relocation
    {
        Relocation(std::string symbol, std::string location, std::string section, int offset, int addend)
            : symbol(std::move(symbol))
            , location(std::move(location))
            , section(std::move(section))
            , offset(offset)
            , addend(addend)
        {
        }

        std::string symbol;
        std::string location;
        std::string section;
        int offset;
        int addend;
    };

    struct DebugAbbrevItem
    {
        uint64_t attribute;
        uint64_t type;
    };

    struct DebugAbbrevEntry
    {
        int id;
        uint32_t type;
        bool hasChildren;
        std::vector<DebugAbbrevItem> info;
    };

    struct DebugInfoEntry
    {
        DebugAbbrevEntry* abbrev;
        std::vector<std::variant<uint8_t, uint16_t, uint32_t, uint64_t, ULEB128, SLEB128, Relocation, TypeOffset> > info;
    };

    class DIBuilder
    {
    friend class Module;
    public:
        DIBuilder(Module& module);

        void setProducer(std::string_view producer);
        void setFilename(std::string_view filename);
        void setDirectory(std::string_view directory);

        void setSourceInfo(Value* value, int line, int col);
        void setSourceInfo(Function* func, int line, int col, int endLine, int endCol);
        void setDebugType(Value* value, DIType* type);

        DIType* createBasicType(std::string name, Type* type, uint8_t encoding);
        DIType* createPointerType(DIType* base);
        DIType* createStructureType(std::string name, StructType* structType, int line, int col);

        DIVariable* createLocalVariable(std::string name, Function* parent, DIType* type, int line, int col);
        DIVariable* createParameterVariable(std::string name, Function* parent, DIType* type);

    private:
        std::string mProducer;
        std::string mFilename;
        std::string mDirectory;

        std::vector<std::unique_ptr<DIType> > mDebugTypes;
        
        int mAbbrevCount;
        bool mFinalized;

        std::unordered_map<std::string, std::uint32_t> mStrings;
        std::unordered_map<std::string, std::uint32_t> mLineStrings;
        

        Module& mModule;
        codegen::OpcodeBuilder* mOpcodeBuilder;

        std::uint32_t getStringPosition(std::string str);
        std::uint32_t getLineStringPosition(std::string str);
        void initializeStringTable();
        uint32_t getDebugInfoSize();
        int getNextAbbrevId();

        void writeULEB128(uint64_t value, std::string section);
        void writeSLEB128(int64_t value,  std::string section);
        void writeTypeOffset(DIType* type, std::string section);

        void createDwarfHeader();
        void createAbbrevEntry(DebugAbbrevEntry entry);
        void createInfoEntry(DebugInfoEntry entry);

        void writeSpecialOpcode(int lineInc, int addrInc);
        void writeStandardOpcode(uint8_t opcode, std::vector<std::variant<ULEB128, SLEB128> > operands);
        void incrementSourceLocation(int lineChange, int addressChange);
        void createDebugLine(std::vector<MC::EmitSourceInfo*>& sourceInfo);

        void createDebugArranges();
        void createDebugAddr();
        void createDebugLoclists();

        void generateDwarf(MC::Builder& mcBuilder, codegen::OpcodeBuilder& opcodeBuilder);

    };
}

#endif // VIPIR_DI_DI_BUILDER_H