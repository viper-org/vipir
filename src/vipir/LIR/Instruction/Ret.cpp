// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Ret.h"

#include "vasm/instruction/NoOperandInstruction.h"
#include "vasm/instruction/singleOperandInstruction/PopInstruction.h"

#include "vasm/instruction/operand/Register.h"


namespace vipir
{
    namespace lir
    {
        Ret::Ret(bool leave, std::vector<int> calleeSaved)
            : mLeave(leave)
            , mCalleeSaved(std::move(calleeSaved))
        {
        }

        void Ret::print(std::ostream& stream) const
        {
            stream << "RET\n";
        }

        void Ret::emit(MC::Builder& builder)
        {
            for (auto calleeSaved : mCalleeSaved)
            {
                instruction::OperandPtr reg = std::make_unique<instruction::Register>(calleeSaved, codegen::OperandSize::Quad);
                builder.addValue(std::make_unique<instruction::PopInstruction>(std::move(reg)));
            }

            if (mLeave)
            {
                builder.addValue(std::make_unique<instruction::LeaveInstruction>());
            }

            builder.addValue(std::make_unique<instruction::RetInstruction>());
        }

        void Ret::setLeave(bool leave)
        {
            mLeave = leave;
        }

        void Ret::setCalleeSaved(std::vector<int> calleeSaved)
        {
            mCalleeSaved = std::move(calleeSaved);
        }
    }
}