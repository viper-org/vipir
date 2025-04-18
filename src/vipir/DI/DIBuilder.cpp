// Copyright 2025 solar-mist


#include "vipir/DI/DIBuilder.h"

#include "vipir/IR/Function.h"
#include "vipir/Module.h"

#include <vasm/codegen/builder/OpcodeBuilder.h>

#include <cstring>
#include <dwarf.h>

namespace vipir
{
    DIType::DIType(std::string name, Type* type, uint8_t encoding)
        : mName(std::move(name))
        , mType(type)
        , mEncoding(encoding)
    {
    }

    DIBuilder::DIBuilder(Module& module)
        : mModule(module)
        , mFinalized(false)
        , mAbbrevCount(1)
    {
    }

    void DIBuilder::setProducer(std::string_view producer)
    {
        mProducer = std::string(producer);
    }

    void DIBuilder::setFilename(std::string_view filename)
    {
        mFilename = std::string(filename);
    }

    void DIBuilder::setDirectory(std::string_view directory)
    {
        mDirectory = std::string(directory);
    }

    void DIBuilder::setSourceInfo(Value* value, int line, int col)
    {
        value->mLine = line;
        value->mCol = col;
    }

    void DIBuilder::setSourceInfo(Function* func, int line, int col, int endLine, int endCol)
    {
        setSourceInfo(func, line, col);
        func->mEndLine = endLine;
        func->mEndCol = endCol;
    }

    void DIBuilder::setDebugType(Value* value, DIType* type)
    {
        value->mDebugType = type;
    }

    DIType* DIBuilder::createDebugType(std::string name, Type* type, uint8_t encoding)
    {
        auto ptr = new DIType(std::move(name), type, encoding);
        mDebugTypes.push_back(std::unique_ptr<DIType>(ptr));
        return ptr;
    }

    DIVariable* DIBuilder::createDebugVariable(std::string name, Function* parent, Value* value, DIType* type, int line, int col)
    {
        auto ptr = new DIVariable(std::move(name), parent, value, type, line, col);
        parent->mDebugVariables.emplace_back(ptr);
        return ptr;
    }


    static inline int EncodeULEB128(uint64_t value, char(&out)[16])
    {
        std::memset(out, 0, 16);
        int idx = 0;
        do {
            auto byte = value & 0x7f;
            value >>= 7;
            if (value) byte |= 0x80;
            out[idx++] = byte;
        } while (value);
        return idx;
    }
    static inline int EncodeSLEB128(int64_t value, char(&out)[16])
    {
        std::memset(out, 0, 16);

        bool negative = value < 0;
        int idx = 0;
        while (true)
        {
            auto byte = value & 0x7f;
            value >>= 7;
            if (negative) value |= (~0ull << 57);
            if ((value == 0 && !(byte & 0x40)) || (value == -1 && byte & 0x40))
            {
                out[idx++] = byte;
                return idx;
            }
            out[idx++] = byte | 0x80;
        }
    }


    std::uint32_t DIBuilder::getStringPosition(std::string str)
    {
        if (mStrings.find(str) != mStrings.end())
        {
            return mStrings[str];
        }

        auto position = mOpcodeBuilder->getPosition(".debug_str");
        mOpcodeBuilder->createInstruction(".debug_str")
            .string(str, true)
            .emit();
        
        mStrings[str] = position;
        return position;
    }

    std::uint32_t DIBuilder::getLineStringPosition(std::string str)
    {
        if (mLineStrings.find(str) != mLineStrings.end())
        {
            return mLineStrings[str];
        }

        auto position = mOpcodeBuilder->getPosition(".debug_line_str");
        mOpcodeBuilder->createInstruction(".debug_line_str")
            .string(str, true)
            .emit();
        
        mLineStrings[str] = position;
        return position;
    }

    void DIBuilder::initializeStringTable()
    {
    }

    uint32_t DIBuilder::getDebugInfoSize()
    {
        // Any references into .debug_info will be 4 bytes off before the length field is emitted,
        // so that is checked here
        return mOpcodeBuilder->getPosition(".debug_info") + (mFinalized ? 0 : 4);
    }

    int DIBuilder::getNextAbbrevId()
    {
        return mAbbrevCount++;
    }

    void DIBuilder::writeULEB128(uint64_t value, std::string section)
    {
        char temp[16];
        auto len = EncodeULEB128(value, temp);
        
        mOpcodeBuilder->createInstruction(section)
            .string(std::string_view(temp, temp + len))
            .emit();
    }

    void DIBuilder::writeSLEB128(int64_t value, std::string section)
    {
        char temp[16];
        auto len = EncodeSLEB128(value, temp);
        
        mOpcodeBuilder->createInstruction(section)
            .string(std::string_view(temp, temp + len))
            .emit();
    }

    void DIBuilder::createDwarfHeader()
    {
        // format
        mOpcodeBuilder->createInstruction(".debug_info")
            .immediate((uint16_t)0x5)
            .emit();

        // unit type
        mOpcodeBuilder->createInstruction(".debug_info")
            .immediate((uint8_t)0x1)
            .emit();
        

        // address size
        mOpcodeBuilder->createInstruction(".debug_info")
            .immediate((uint8_t)0x8)
            .emit();

        // offset into .debug_abbrev
        mOpcodeBuilder->createInstruction(".debug_info")
            .immediate((uint32_t)0)
            .emit();
    }

    void DIBuilder::createAbbrevEntry(DebugAbbrevEntry entry)
    {
        char temp[16];

        writeULEB128(entry.id, ".debug_abbrev");
        writeULEB128(entry.type, ".debug_abbrev");
        
        mOpcodeBuilder->createInstruction(".debug_abbrev")
            .immediate((uint8_t)(entry.hasChildren ? DW_CHILDREN_yes : DW_CHILDREN_no))
            .emit();

        for (auto& info : entry.info)
        {
            writeULEB128(info.attribute, ".debug_abbrev");
            writeULEB128(info.type, ".debug_abbrev");
        }

        mOpcodeBuilder->createInstruction(".debug_abbrev")
            .immediate((uint16_t)0)
            .emit();
    }

    // Helper types from cppreference.com
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    void DIBuilder::createInfoEntry(DebugInfoEntry entry)
    {
        char temp[16];

        writeULEB128(entry.abbrev->id, ".debug_info");

        for (auto info : entry.info)
        {
            std::visit(overloaded{
                [this, &temp](ULEB128 arg) {
                    writeULEB128(arg.mValue, ".debug_info");
                },
                [this, &temp](SLEB128 arg) {
                    writeSLEB128(arg.mValue, ".debug_info");
                },
                [this](Relocation arg) {
                    mOpcodeBuilder->relocLabel(arg.symbol, arg.location, arg.section, arg.offset, arg.addend);
                    mOpcodeBuilder->createInstruction(".debug_info")
                        .immediate(mOpcodeBuilder->getLabel(arg.symbol).first)
                        .emit();
                },
                [this](auto arg) { 
                    mOpcodeBuilder->createInstruction(".debug_info")
                        .immediate(arg)
                        .emit();
                }
            }, info);
        }
    }

    uint8_t EncodeSpecialOpcode(int lineInc, int opAdv)
    {
        uint32_t ret = (lineInc + 5) + (14 * opAdv) + 13;
        if (ret > 255) return 0;
        return ret;
    }

    void DIBuilder::writeSpecialOpcode(int lineInc, int addrInc)
    {
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate(EncodeSpecialOpcode(lineInc, addrInc))
            .emit();
    }

    void DIBuilder::writeStandardOpcode(uint8_t opcode, std::vector<std::variant<ULEB128, SLEB128> > operands)
    {
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate(opcode)
            .emit();
        for (auto& operand : operands)
        {
            std::visit(overloaded {
                [this](ULEB128 arg) { writeULEB128(arg.mValue, ".debug_line"); },
                [this](SLEB128 arg) { writeSLEB128(arg.mValue, ".debug_line"); },
            }, operand);
        }
    }

    void DIBuilder::incrementSourceLocation(int lineChange, int addressChange)
    {
        auto specialOpcode = EncodeSpecialOpcode(lineChange, addressChange);
        if (!specialOpcode)
        {
            writeStandardOpcode(DW_LNS_advance_line, {SLEB128(lineChange)});
            writeStandardOpcode(DW_LNS_advance_pc, {ULEB128(addressChange)});
            writeStandardOpcode(DW_LNS_copy, {});
        }
        else
        {
            writeSpecialOpcode(lineChange, addressChange);
        }
    }

    void DIBuilder::createDebugLine(std::vector<MC::EmitSourceInfo*>& sourceInfo)
    {
        mOpcodeBuilder->setSection(".debug_line");
        // version
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint16_t)0x5)
            .emit();

        // address size
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)0x8)
            .emit();

        // segment selector size
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)0)
            .emit();

        auto headerLengthPos = mOpcodeBuilder->getPosition(".debug_line");

        // min inst length
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)1)
            .emit();

        // max ops per inst
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)1)
            .emit();

        // default is stmt
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)1)
            .emit();

        // Maybe generate these depending on the line information?
        // line base
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)(-5))
            .emit();

        // line range
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)14)
            .emit();

        // opcode base
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)13)
            .emit();

        uint8_t standardOpcodeLengths[] = { 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 };
        for (auto length : standardOpcodeLengths)
        {
            mOpcodeBuilder->createInstruction(".debug_line")
                .immediate(length)
                .emit();
        }

        // directory entry format count
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)1)
            .emit();
        
        // directory entry format
        writeULEB128(DW_LNCT_path, ".debug_line");
        writeULEB128(DW_FORM_line_strp, ".debug_line"); // Should debug line str be used here?

        // TODO: Iterate over provided directories
        // number of directories
        writeULEB128(1, ".debug_line");
        // compilation directory
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate(getLineStringPosition(mDirectory))
            .emit();

        // filename entry format count
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)1)
            .emit();
        
        // filename entry format
        writeULEB128(DW_LNCT_path, ".debug_line");
        writeULEB128(DW_FORM_line_strp, ".debug_line"); // Should debug line str be used here?

        // TODO: Iterate over provided filenames?
        // number of filenames
        writeULEB128(1, ".debug_line");
        // source filename
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate(getLineStringPosition(mFilename))
            .emit();
        
        // header length
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint32_t)mOpcodeBuilder->getPosition(".debug_line")-4)
            .emit(headerLengthPos);

        // TODO: Generate line number program
        
        int lastLine = 1;
        int lastCol = 1;
        int lastAddress = 0;
        writeStandardOpcode(DW_LNS_set_file, {0_uleb});
        writeStandardOpcode(DW_LNS_set_column, {1_uleb});

        auto pos = mOpcodeBuilder->getPosition(".debug_line");
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)0)
            .emit();
        writeULEB128(9, ".debug_line");
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)DW_LNE_set_address)
            .emit();
        mOpcodeBuilder->relocLabel(".text", "dwarf", ".debug_line", 4, -4 );
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint64_t)mOpcodeBuilder->getLabel(".text").first)
            .emit();

        for (auto sourceLoc : sourceInfo)
        {
            auto lineDiff = sourceLoc->mLine - lastLine;
            auto colDiff = sourceLoc->mCol - lastCol;
            auto addrDiff = sourceLoc->mAddress - lastAddress;

            if (colDiff)
                writeStandardOpcode(DW_LNS_set_column, {ULEB128(sourceLoc->mCol)});
            incrementSourceLocation(lineDiff, addrDiff);

            lastLine = sourceLoc->mLine;
            lastCol = sourceLoc->mCol;
            lastAddress = sourceLoc->mAddress;
        }

        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)0)
            .emit();
        writeULEB128(1, ".debug_line");
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint8_t)DW_LNE_end_sequence)
            .emit();

        // unit length
        mOpcodeBuilder->createInstruction(".debug_line")
            .immediate((uint32_t)mOpcodeBuilder->getPosition(".debug_line"))
            .emit(0);
    }

    void DIBuilder::createDebugArranges()
    {
        // version
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint16_t)2)
            .emit();

        // debug info offset
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint32_t)0)
            .emit();

        // address size
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint8_t)0x8)
            .emit();
        
        // segment selector size
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint8_t)0)
            .emit();
        
        // .text
        mOpcodeBuilder->relocLabel(".text", "dwarf", ".debug_arranges", 4, -4);
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint64_t)0)
            .emit();
        
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint64_t)mOpcodeBuilder->getPosition(".text"))
            .emit();
        
        // null entry (end)
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint64_t)0)
            .emit();
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint64_t)0)
            .emit();
        
        // unit length
        mOpcodeBuilder->createInstruction(".debug_arranges")
            .immediate((uint32_t)mOpcodeBuilder->getPosition(".debug_arranges"))
            .emit(0);
    }

    void DIBuilder::generateDwarf(MC::Builder& mcBuilder, codegen::OpcodeBuilder& opcodeBuilder)
    {
        mOpcodeBuilder = &opcodeBuilder;
        mOpcodeBuilder->addLabel(".LEtext", ".text", false);

        initializeStringTable();
        
        uint32_t filenamePosition = getStringPosition(mFilename);
        uint32_t directoryPosition = getStringPosition(mDirectory);
        uint32_t producerPosition = getStringPosition(mProducer);

        createDwarfHeader();
        
        /*
         * Compile Unit
         */
        DebugAbbrevEntry compileUnit = { getNextAbbrevId(), DW_TAG_compile_unit, true, {
            { DW_AT_producer, DW_FORM_strp },
            { DW_AT_language, DW_FORM_data1 },
            { DW_AT_name, DW_FORM_strp },
            { DW_AT_comp_dir, DW_FORM_strp },
            { DW_AT_low_pc, DW_FORM_addr },
            { DW_AT_high_pc, DW_FORM_data8 },
            { DW_AT_stmt_list, DW_FORM_sec_offset },
        }};
        createAbbrevEntry(compileUnit);
        
        DebugInfoEntry compileUnitInfo = { &compileUnit, {
            producerPosition,
            (uint8_t)DW_LANG_C11,
            filenamePosition,
            directoryPosition,
            Relocation(".text", "dwarf", ".debug_info", 4, -4),
            opcodeBuilder.getPosition(".text"),
            (uint32_t)0
        }};
        createInfoEntry(compileUnitInfo);

        DebugAbbrevEntry typeAbbrev = { getNextAbbrevId(), DW_TAG_base_type, false, {
            { DW_AT_byte_size, DW_FORM_data1 },
            { DW_AT_encoding, DW_FORM_data1 },
            { DW_AT_name, DW_FORM_strp },
        }};
        createAbbrevEntry(typeAbbrev);

        for (auto& type : mDebugTypes)
        {
            type->mOffset = getDebugInfoSize();
            DebugInfoEntry typeInfo = { &typeAbbrev, {
                (uint8_t)(type->mType->getSizeInBits() / 8),
                (uint8_t)type->mEncoding,
                getStringPosition(type->mName)
            }};
            createInfoEntry(typeInfo);
        }

        DebugAbbrevEntry variableAbbrev = { getNextAbbrevId(), DW_TAG_variable, false, {
            { DW_AT_name, DW_FORM_strp },
            { DW_AT_decl_file, DW_FORM_strp },
            { DW_AT_decl_line, DW_FORM_data2 },
            { DW_AT_decl_column, DW_FORM_data2 },
            { DW_AT_type, DW_FORM_ref4 },
            { DW_AT_location, DW_FORM_exprloc },
        }};
        createAbbrevEntry(variableAbbrev);
        
        int idx = 2;
        for (auto& global : mModule.getGlobals())
        {
            if (auto func = dynamic_cast<Function*>(global.get()))
            {
                auto emittedValue = global->getEmittedValue();
                auto label = static_cast<lir::Lbl*>(emittedValue.get());
                
                std::string endSym = opcodeBuilder.getLabelAfter(label->getName());
                if (endSym.empty()) endSym = ".LEtext";

                uint64_t start = opcodeBuilder.getLabel(label->getName()).first;
                uint64_t end = opcodeBuilder.getLabel(endSym).first;
                if (end == 0) end = opcodeBuilder.getPosition(".text");

                auto type = global->mDebugType;
                uint32_t typeOffset = type->mOffset;

                bool hasChildren = false;
                if (!func->mDebugVariables.empty()) hasChildren = true;

                DebugAbbrevEntry subprogramAbbrev = { getNextAbbrevId(), DW_TAG_subprogram, hasChildren, {
                    { DW_AT_external, DW_FORM_flag_present },
                    { DW_AT_name, DW_FORM_strp },
                    { DW_AT_decl_file, DW_FORM_data1 },
                    { DW_AT_decl_line, DW_FORM_data1 },
                    { DW_AT_decl_column, DW_FORM_data1 },
                    { DW_AT_type, DW_FORM_ref4 },
                    { DW_AT_low_pc, DW_FORM_addr },
                    { DW_AT_high_pc, DW_FORM_data8 },
                    { DW_AT_frame_base, DW_FORM_exprloc },
                    { DW_AT_call_all_calls, DW_FORM_flag_present },
                }};
                createAbbrevEntry(subprogramAbbrev);
                
                DebugInfoEntry subprogramInfoStart = { &subprogramAbbrev, {
                    getStringPosition(label->getName()),
                    (uint8_t)0,
                    (uint8_t)global->mLine,
                    (uint8_t)global->mCol,
                    typeOffset,
                    Relocation(".text", "dwarf", ".debug_info", 4, start - 4),
                    end - start,
                    ULEB128(1),
                    (uint8_t)DW_OP_call_frame_cfa
                }};
                createInfoEntry(subprogramInfoStart);

                for (auto& debugVariable : func->mDebugVariables)
                {
                    DebugInfoEntry variableInfo = { &variableAbbrev, {
                        getStringPosition(debugVariable->mName),
                        getStringPosition(mFilename),
                        (uint16_t)debugVariable->mLine,
                        (uint16_t)debugVariable->mCol,
                        (uint32_t)debugVariable->mType->mOffset,
                    }};
                    auto value = debugVariable->mValue->getEmittedValue();
                    if (auto imm = dynamic_cast<lir::Immediate*>(value.get()))
                    {
                        variableInfo.info.push_back(ULEB128(10));
                        variableInfo.info.push_back((uint8_t)DW_OP_implicit_value);
                        variableInfo.info.push_back(ULEB128(8));
                        variableInfo.info.push_back((uint64_t)imm->value());
                    }
                    if (auto vreg = dynamic_cast<lir::VirtualReg*>(value.get()))
                    {
                        if (vreg->mVreg->onStack())
                        {
                            variableInfo.info.push_back(ULEB128(2));
                            variableInfo.info.push_back((uint8_t)DW_OP_fbreg);
                            variableInfo.info.push_back(SLEB128(-vreg->mVreg->getStackOffset() - 16));
                        }
                    }
                    // TODO: Potential other locations here
                    createInfoEntry(variableInfo);
                }

                // Emit a null byte to signify this is the end of this entry's children
                if (hasChildren)
                {
                    opcodeBuilder.createInstruction(".debug_info")
                        .immediate((uint8_t)0)
                        .emit();
                }
            }
        }

        opcodeBuilder.createInstruction(".debug_info")
            .immediate((uint8_t)0)
            .emit();
        opcodeBuilder.createInstruction(".debug_info")
            .immediate((uint8_t)0)
            .emit();
        opcodeBuilder.createInstruction(".debug_abbrev")
            .immediate((uint8_t)0)
            .emit();

        createDebugLine(mcBuilder.getSourceInfo());
        //createDebugArranges();
        
        // unit length
        opcodeBuilder.createInstruction(".debug_info")
            .immediate((uint32_t)opcodeBuilder.getPosition(".debug_info"))
            .emit(0);
        mFinalized = true;
    }
}