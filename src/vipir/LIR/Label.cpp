// Copyright 2024 solar-mist


#include "vipir/LIR/Label.h"

#include "vasm/instruction/Label.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        Label::Label(std::string name, bool global)
            : mName(name)
            , mGlobal(global)
        {
        }

        void Label::print(std::ostream& stream) const
        {
            stream << std::format("LABEL {}\n", mName);
        }

        void Label::emit(MC::Builder& builder)
        {
            builder.addValue(std::make_unique<instruction::Label>(mName, mGlobal));
        }
    }
}