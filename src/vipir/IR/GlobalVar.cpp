// Copyright 2024 solar-mist


#include "vipir/IR/GlobalVar.h"

#include "vipir/Module.h"

#include "vipir/Type/ArrayType.h"
#include "vipir/Type/StructType.h"
#include "vipir/Type/PointerType.h"

#include "vipir/MC/CompoundOperand.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/operand/Label.h"

#include "vasm/instruction/singleOperandInstruction/DeclInstruction.h"
#include "vasm/instruction/Directive.h"

#include <algorithm>
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
        builder.addValue(std::make_unique<instruction::SectionDirective>(".data"));

        mEmittedValue = std::make_unique<instruction::LabelOperand>(std::to_string(mValueId));
        builder.addValue(std::make_unique<instruction::Label>(std::to_string(mValueId)));

        instruction::OperandPtr initVal = mInitialValue->getEmittedValue();

        emitConstant(builder, mType->getOperandSize(), std::move(initVal));
    }

    void GlobalVar::emitConstant(MC::Builder& builder, codegen::OperandSize size, instruction::OperandPtr value)
    {
        if (auto compoundOperand = dynamic_cast<CompoundOperand*>(value.get()))
        {
            codegen::OperandSize size;
            if (auto arrayType = dynamic_cast<ArrayType*>(static_cast<PointerType*>(mType)->getBaseType()))
            {
                size = arrayType->getBaseType()->getOperandSize();
            }
            else
            {
                StructType* structType = static_cast<StructType*>(static_cast<PointerType*>(mType)->getBaseType());
                auto it = std::max_element(structType->getFields().begin(), structType->getFields().end(), [](auto a, auto b){
                    return a->getSizeInBits() < b->getSizeInBits();
                });
                size = (*it)->getOperandSize();
            }
            for (auto& value : compoundOperand->getValues())
            {
                emitConstant(builder, size, std::move(value));
            }
        }
        else
        {
            switch(size)
            {
                case codegen::OperandSize::Byte:
                    builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Byte> >(std::move(value)));
                    break;
                case codegen::OperandSize::Word:
                    builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Word> >(std::move(value)));
                    break;
                case codegen::OperandSize::Long:
                    builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Long> >(std::move(value)));
                    break;
                case codegen::OperandSize::Quad:
                    builder.addValue(std::make_unique<instruction::DeclInstruction<codegen::OperandSize::Quad> >(std::move(value)));
                    break;
                case codegen::OperandSize::None:
                    break;
            }
        }
    }

    void GlobalVar::setInitialValue(Value* value)
    {
        assert(value->isConstant());
        mInitialValue = value;
    }

    GlobalVar::GlobalVar(Module& module, Type* type)
        : Global(module)
        , mValueId(module.getNextValueId())
    {
        mType = Type::GetPointerType(type);
    }
}