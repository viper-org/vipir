// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/EnterFunc.h"

#include "vipir/MC/CmpOperand.h"

#include "vasm/instruction/twoOperandInstruction/MovInstruction.h"
#include "vasm/instruction/twoOperandInstruction/LogicalInstruction.h"
#include "vasm/instruction/singleOperandInstruction/PushInstruction.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Immediate.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        EnterFunc::EnterFunc(int stackSize, std::vector<int> calleeSaved)
            : mStackSize(stackSize)
            , mCalleeSaved(std::move(calleeSaved))
        {
        }

        void EnterFunc::print(std::ostream& stream) const
        {
            stream << std::format("ENTER FN\n");
        }

        void EnterFunc::emit(MC::Builder& builder)
        {
            if (mSaveFramePointer)
            {
                builder.addValue(std::make_unique<instruction::PushInstruction>(instruction::Register::Get("rbp")));
                builder.addValue(std::make_unique<instruction::MovInstruction>(instruction::Register::Get("rbp"), instruction::Register::Get("rsp")));
                if (!mStackSize)
                {
                    instruction::OperandPtr rsp = instruction::Register::Get("rsp");
                    instruction::OperandPtr stackOffset = std::make_unique<instruction::Immediate>(8);
                    builder.addValue(std::make_unique<instruction::SubInstruction>(std::move(rsp), std::move(stackOffset)));
                }
            }

            if (mStackSize)
            {
                instruction::OperandPtr reg = instruction::Register::Get("rsp");
                instruction::OperandPtr stackOffset = std::make_unique<instruction::Immediate>(mStackSize);
                builder.addValue(std::make_unique<instruction::SubInstruction>(std::move(reg), std::move(stackOffset)));
            }

            for (auto calleeSaved : mCalleeSaved)
            {
                instruction::OperandPtr reg = std::make_unique<instruction::Register>(calleeSaved, codegen::OperandSize::Quad);
                builder.addValue(std::make_unique<instruction::PushInstruction>(std::move(reg)));
            }
        }

        void EnterFunc::setStackSize(int newSize)
        {
            mStackSize = newSize;
        }

        void EnterFunc::setCalleeSaved(std::vector<int> calleeSaved)
        {
            mCalleeSaved = std::move(calleeSaved);
        }

        void EnterFunc::setSaveFramePointer(bool saveFramePointer)
        {
            mSaveFramePointer = saveFramePointer;
        }
    }
}