// Copyright 2024 solar-mist


#include "vipir/IR/GlobalVar.h"

#include "vipir/Module.h"

#include "vipir/Type/ArrayType.h"
#include "vipir/Type/StructType.h"
#include "vipir/Type/PointerType.h"

#include "vipir/MC/CompoundOperand.h"

#include "vipir/LIR/Label.h"
#include "vipir/LIR/Global.h"

#include "vasm/instruction/Label.h"
#include "vasm/instruction/operand/Label.h"

#include "vasm/instruction/singleOperandInstruction/DeclInstruction.h"
#include "vasm/instruction/Directive.h"

#include <algorithm>
#include <format>
#include <ranges>

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


    void GlobalVar::emit(lir::Builder& builder)
    {
        mInitialValue->lateEmit(builder);

        builder.setSection(lir::SectionType::Data);

        mEmittedValue = std::make_unique<lir::Lbl>(std::to_string(mValueId));
        builder.addValue(std::make_unique<lir::Label>(std::to_string(mValueId), false));

        lir::OperandPtr init = mInitialValue->getEmittedValue();

        emitConstant2(builder, static_cast<PointerType*>(mType)->getBaseType(), std::move(init));
    }

    void GlobalVar::emitConstant2(lir::Builder& builder, Type* type, lir::OperandPtr value)
    {
        if (auto compoundOperand = dynamic_cast<lir::Compound*>(value.get()))
        {
            codegen::OperandSize size;
            if (auto arrayType = dynamic_cast<ArrayType*>(type))
            {
                for (auto& value : compoundOperand->getValues())
                {
                    emitConstant2(builder, arrayType->getBaseType(), std::move(value));
                }
            }
            else
            {
                StructType* structType = static_cast<StructType*>(type);

                auto notArray = [](Type* type) { return !type->isArrayType(); };
                auto largestElement = std::max_element(structType->getFields().begin(), structType->getFields().end(), [](auto a, auto b){
                    return a->getSizeInBits() < b->getSizeInBits();
                });
                auto fieldsNotArray = structType->getFields() | std::views::filter(notArray);
                auto largestNotArray = std::max_element(fieldsNotArray.begin(), fieldsNotArray.end(), [](auto a, auto b){
                    return a->getSizeInBits() < b->getSizeInBits();
                });

                int index = 0;
                for (auto& value : compoundOperand->getValues())
                {
                    if ((*largestElement)->isArrayType() && structType->getField(index++) != *largestElement)
                    {
                        emitConstant2(builder, *largestNotArray, std::move(value));
                    }
                    else
                    {
                        emitConstant2(builder, *largestElement, std::move(value));
                    }
                }
            }
        }
        else
        {
            builder.addValue(std::make_unique<lir::GlobalDecl>(std::move(value), type->getOperandSize()));
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