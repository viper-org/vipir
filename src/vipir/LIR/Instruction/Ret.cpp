// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/Ret.h"

#include "vasm/instruction/NoOperandInstruction.h"
#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"
#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/instruction/singleOperandInstruction/PushInstruction.h"
#include "vasm/instruction/singleOperandInstruction/PopInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Immediate.h"


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
            if (mStackSize)
            {
                instruction::OperandPtr rsp = instruction::Register::Get("rsp");
                instruction::OperandPtr stackOffset = std::make_unique<instruction::Immediate>(mStackSize);
                builder.addValue(std::make_unique<instruction::AddInstruction>(std::move(rsp), std::move(stackOffset)));
            }
            for (auto it = mCalleeSaved.rbegin(); it != mCalleeSaved.rend(); ++it)
            {
                auto& calleeSaved = *it;
                instruction::OperandPtr reg = std::make_unique<instruction::Register>(calleeSaved, codegen::OperandSize::Quad);
                builder.addValue(std::make_unique<instruction::PopInstruction>(std::move(reg)));
            }
            if (mSaveFramePointer)
            {
                instruction::OperandPtr rbp = instruction::Register::Get("rbp");
                builder.addValue(std::make_unique<instruction::PopInstruction>(std::move(rbp)));
                //builder.addValue(std::make_unique<instruction::LeaveInstruction>());
            }

            /*if (!mCalleeSaved.empty())
            {
                if (mLeave)
                    builder.addValue(std::make_unique<instruction::LeaveInstruction>());
                else
                {
                    instruction::OperandPtr rsp = instruction::Register::Get("rsp");
                    instruction::OperandPtr stackOffset = std::make_unique<instruction::Immediate>(8);
                    builder.addValue(std::make_unique<instruction::AddInstruction>(std::move(rsp), std::move(stackOffset)));
                }
            }
            else
            {
                if (mSaveFramePointer)
                {
                    if (mLeave)
                        builder.addValue(std::make_unique<instruction::PopInstruction>(instruction::Register::Get("rbp")));
                    else
                    {
                        instruction::OperandPtr rsp = instruction::Register::Get("rsp");
                        instruction::OperandPtr stackOffset = std::make_unique<instruction::Immediate>(8);
                        builder.addValue(std::make_unique<instruction::AddInstruction>(std::move(rsp), std::move(stackOffset)));
                    }
                }
            }*/

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

        void Ret::setStackSize(int stackSize)
        {
            mStackSize = stackSize;
        }

        void Ret::setSaveFramePointer(bool saveFramePointer)
        {
            mSaveFramePointer = saveFramePointer;
        }
    }
}