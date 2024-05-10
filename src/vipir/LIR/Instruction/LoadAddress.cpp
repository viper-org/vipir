// Copyright 2024 solar-mist


#include "vipir/LIR/Instruction/LoadAddress.h"

#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"

#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/Relative.h"
#include "vipir/LIR/Operand.h"

#include <cassert>
#include <format>

namespace vipir
{
    namespace lir
    {
        LoadAddress::LoadAddress(OperandPtr left, OperandPtr right)
            : mLeft(std::move(left))
            , mRight(std::move(right))
        {
            assert(mRight->isMemory() || dynamic_cast<Lbl*>(mRight.get()));
        }

        void LoadAddress::print(std::ostream& stream) const
        {
            stream << std::format("LA {} -> {}\n", mRight->ident(), mLeft->ident());
        }

        void LoadAddress::emit(MC::Builder& builder)
        {
            instruction::OperandPtr right = mRight->asmOperand();
            if (auto label = dynamic_cast<instruction::LabelOperand*>(right.get()))
            {
                std::string name = std::string(label->getName());
                std::string location = std::string(label->getLocation());
                instruction::LabelOperandPtr labelPtr = std::make_unique<instruction::LabelOperand>(std::move(name), std::move(location));
                right = std::make_unique<instruction::Relative>(std::move(labelPtr), std::nullopt);
            }

            builder.addValue(std::make_unique<instruction::LeaInstruction>(mLeft->asmOperand(), std::move(right)));
        }

        std::vector<std::reference_wrapper<OperandPtr> > LoadAddress::getOutputOperands()
        {
            return {mLeft};
        }

        std::vector<std::reference_wrapper<OperandPtr> > LoadAddress::getInputOperands()
        {
            return {mRight};
        }
    }
}