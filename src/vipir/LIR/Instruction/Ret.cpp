// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Ret.h"

#include "vasm/instruction/NoOperandInstruction.h"


namespace vipir
{
    namespace lir
    {
        Ret::Ret()
        {
        }

        void Ret::print(std::ostream& stream) const
        {
            stream << "RET\n";
        }

        void Ret::emit(MC::Builder& builder)
        {
            builder.addValue(std::make_unique<instruction::RetInstruction>());
        }
    }
}