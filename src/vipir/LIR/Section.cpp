// Copyright 2024 solar-mist


#include "vipir/LIR/Section.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/Directive.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        Section::Section(SectionType type)
            : mType(type)
        {
        }

        void Section::print(std::ostream&) const
        {
        }

        void Section::emit(MC::Builder& builder)
        {
            std::string section;
            switch (mType)
            {
                case SectionType::Code:
                    section = ".text";
                    break;
                case SectionType::Data:
                    section = ".data";
                    break;
            }

            builder.addValue(std::make_unique<instruction::SectionDirective>(section));
        }
    }
}