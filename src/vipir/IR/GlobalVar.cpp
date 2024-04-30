// Copyright 2024 solar-mist


#include "vipir/IR/GlobalVar.h"

#include "vipir/Module.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/operand/Label.h"

#include "vasm/instruction/singleOperandInstruction/DeclInstruction.h"

#include <format>

namespace vipir
{
    void GlobalVar::print(std::ostream& stream)
    {
        stream << std::format("\n{} @{} = {}", mType->getName(), mValueId, mInitialValue->ident());
    }

    std::string GlobalVar::ident() const
    {
        return std::format("{} @{}", mType->getName(), mValueId);
    }

    void GlobalVar::emit(MC::Builder& builder)
    {
        mEmittedValue = std::make_unique<instruction::LabelOperand>(std::to_string(mValueId));
        builder.addValue(std::make_unique<instruction::Label>(std::to_string(mValueId)));

        mInitialValue->emit(builder);
        instruction::OperandPtr initVal = mInitialValue->getEmittedValue();
        switch(mType->getOperandSize())
        {
            case codegen::OperandSize::Byte:
                builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Byte> >(std::move(initVal)));
                break;
            case codegen::OperandSize::Word:
                builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Word> >(std::move(initVal)));
                break;
            case codegen::OperandSize::Long:
                builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Long> >(std::move(initVal)));
                break;
            case codegen::OperandSize::Quad:
                builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Quad> >(std::move(initVal)));
                break;
            case codegen::OperandSize::None:
                break; // TODO: Add struct initial value support
        }
    }

    void GlobalVar::setInitialValue(Value* value)
    {
        assert(value->isConstant());
        mInitialValue = std::unique_ptr<Value>(value);
    }

    GlobalVar::GlobalVar(Module& module, Type* type)
        : Global(module)
        , mValueId(module.getNextValueId())
    {
        mType = Type::GetPointerType(type);
    }
}