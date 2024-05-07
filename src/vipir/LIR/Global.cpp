// Copyright 2024 solar-mist


#include "vipir/LIR/Global.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/Directive.h"
#include "vasm/instruction/operand/String.h"
#include "vasm/instruction/singleOperandInstruction/DeclInstruction.h"

#include <format>

namespace vipir
{
    namespace lir
    {
        GlobalString::GlobalString(std::string value)
            : mValue(std::move(value))
        {
        }

        void GlobalString::print(std::ostream& stream) const
        {
            stream << std::format("STRING \"{}\"\n", mValue);
        }

        void GlobalString::emit(MC::Builder& builder)
        {
            instruction::OperandPtr string = std::make_unique<instruction::String>(mValue);
            builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Byte> >(std::move(string)));
        }

        GlobalDecl::GlobalDecl(OperandPtr value, codegen::OperandSize size)
            : mValue(std::move(value))
            , mSize(size)
        {
        }

        void GlobalDecl::print(std::ostream& stream) const
        {
            stream << std::format("DECL {}", mValue->ident());
        }

        void GlobalDecl::emit(MC::Builder& builder)
        {
            switch(mSize)
            {
                case codegen::OperandSize::Byte:
                    builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Byte> >(mValue->asmOperand()));
                    break;
                case codegen::OperandSize::Word:
                    builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Word> >(mValue->asmOperand()));
                    break;
                case codegen::OperandSize::Long:
                    builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Long> >(mValue->asmOperand()));
                    break;
                case codegen::OperandSize::Quad:
                    builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Quad> >(mValue->asmOperand()));
                    break;
                case codegen::OperandSize::None:
                    break;
            }
        }
    }
}