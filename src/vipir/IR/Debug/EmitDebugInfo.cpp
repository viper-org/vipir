// Copyright 2025 solar-mist


#include "vipir/IR/Debug/EmitDebugInfo.h"

namespace vipir
{
    void EmitDebugInfo::print(std::ostream& stream)
    {
        stream << std::format(".loc {}:{}", mLine, mCol);
    }

    std::string EmitDebugInfo::ident() const
    {
        return "%undef";
    }

    void EmitDebugInfo::doConstantFold()
    {
    }


    void EmitDebugInfo::emit(lir::Builder& builder)
    {
        builder.addValue(std::make_unique<lir::EmitSourceInfo>(mLine, mCol));
    }

    EmitDebugInfo::EmitDebugInfo(Module& module, int line, int col)
        : Value(module)
        , mLine(line)
        , mCol(col)
    {
    }
}