// Copyright 2024 solar-mist

#include "vipir/MC/Builder.h"

#include <format>

namespace vipir
{
    namespace MC
    {
        EmitSourceInfo::EmitSourceInfo(int line, int col)
            : mLine(line)
            , mCol(col)
        {
        }

        void EmitSourceInfo::emit(codegen::OpcodeBuilder& builder, codegen::Section section)
        {
            mAddress = builder.getPosition(section);
        }

        void EmitSourceInfo::print(std::ostream& stream)
        {
            stream << std::format(".srcloc {}:{}", mLine, mCol);
        }

        
        Builder::Builder() {}

        void Builder::addValue(instruction::ValuePtr value)
        {
            if (auto sourceInfo = dynamic_cast<EmitSourceInfo*>(value.get()))
            {
                mSourceInfo.push_back(sourceInfo);
            }
            mValues.push_back(std::move(value));
        }

        std::vector<instruction::ValuePtr>& Builder::getValues()
        {
            return mValues;
        }

        std::vector<EmitSourceInfo*>& Builder::getSourceInfo()
        {
            return mSourceInfo;
        }
    }
}