// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Ret.h"

#include "vasm/instruction/NoOperandInstruction.h"


namespace vipir
{
    namespace lir
    {
        Ret::Ret(bool leave)
            : mLeave(leave)
        {
        }

        void Ret::print(std::ostream& stream) const
        {
            stream << "RET\n";
        }

        void Ret::emit(MC::Builder& builder)
        {
            if (mLeave)
            {
                builder.addValue(std::make_unique<instruction::LeaveInstruction>());
            }
            builder.addValue(std::make_unique<instruction::RetInstruction>());
        }
    }
}