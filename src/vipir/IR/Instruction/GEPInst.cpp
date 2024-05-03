// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/GEPInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/Type/ArrayType.h"
#include "vipir/Type/PointerType.h"
#include "vipir/Type/StructType.h"

#include "vipir/Module.h"

#include "vasm/instruction/operand/Register.h"
#include "vasm/instruction/operand/Immediate.h"
#include "vasm/instruction/operand/Memory.h"
#include "vasm/instruction/operand/Label.h"
#include "vasm/instruction/operand/Relative.h"

#include "vasm/instruction/twoOperandInstruction/LeaInstruction.h"

#include <format>

namespace vipir
{
    void GEPInst::print(std::ostream& stream)
    {
        stream << std::format("gep %{}, {}, {}", mValueId, mPtr->ident(), mOffset->ident());
    }

    std::string GEPInst::ident() const
    {
        return std::format("%{}", mValueId);
    }

    std::vector<Value*> GEPInst::getOperands()
    {
        return std::vector<Value*>{mPtr, mOffset};
    }

    void GEPInst::emit(MC::Builder& builder)
    {
        codegen::OperandSize size = mType->getOperandSize();
        instruction::OperandPtr reg = std::make_unique<instruction::Register>(mRegisterID, size);

        instruction::OperandPtr ptr = mPtr->getEmittedValue();
        instruction::OperandPtr offset = mOffset->getEmittedValue();

        instruction::Register* ptrReg = dynamic_cast<instruction::Register*>(ptr.get());
        instruction::Memory* ptrMem = dynamic_cast<instruction::Memory*>(ptr.get());
        instruction::LabelOperand* ptrLabel = dynamic_cast<instruction::LabelOperand*>(ptr.get());
        std::optional<int> displacement;
        if (ptrMem)
        {
            ptrReg = ptrMem->getBase();
            displacement = ptrMem->getDisplacement();
        }

        int scale = mAlignment / 8;

        if (auto immediate = dynamic_cast<instruction::Immediate*>(offset.get()))
        {
            int disp = immediate->imm64() * scale;
            if (displacement) disp += *displacement;
            (void)ptr.release();

            
            if (StructType* structType = dynamic_cast<StructType*>(static_cast<PointerType*>(mPtr->getType())->getBaseType()))
            {
                disp = displacement.value_or(0);
                for (int i = 0; i < immediate->imm64(); ++i)
                {
                    if (structType->getField(i)->isArrayType())
                    {
                        ArrayType* arrayType = static_cast<ArrayType*>(structType->getField(i));
                        disp += std::max(arrayType->getSizeInBits() / 8, (std::size_t)scale);
                    }
                    else
                    {
                        disp += scale;
                    }
                }
            }
            
            instruction::OperandPtr memory;
            if (ptrLabel)
            {
                (void)ptr.release();
                memory = std::make_unique<instruction::Relative>(instruction::LabelOperandPtr(ptrLabel), disp);
            }
            else
            {
                memory = std::make_unique<instruction::Memory>(instruction::RegisterPtr(ptrReg), disp, nullptr, std::nullopt);
            }
            builder.addValue(std::make_unique<instruction::LeaInstruction>(reg->clone(), std::move(memory)));
            mEmittedValue = std::move(reg);
        }
        else if (auto regOffset = dynamic_cast<instruction::Register*>(offset.get()))
        {
            (void)ptr.release();
            instruction::Register* index = regOffset;
            (void)offset.release();

            instruction::OperandPtr memory = std::make_unique<instruction::Memory>(instruction::RegisterPtr(ptrReg), displacement, instruction::RegisterPtr(regOffset), scale);
            builder.addValue(std::make_unique<instruction::LeaInstruction>(reg->clone(), std::move(memory)));
            mEmittedValue = std::move(reg);
        }
    }

    GEPInst::GEPInst(BasicBlock* parent, Value* ptr, Value* offset)
        : Instruction(parent->getModule(), parent)
        , mPtr(ptr)
        , mOffset(offset)
        , mValueId(mModule.getNextValueId())
    {
        if (static_cast<PointerType*>(mPtr->getType())->getBaseType()->isStructType())
        {
            StructType* structType = static_cast<StructType*>(static_cast<PointerType*>(mPtr->getType())->getBaseType());
            ConstantInt* offset = static_cast<ConstantInt*>(mOffset);
            mType = Type::GetPointerType(structType->getField(offset->getValue()));
            mAlignment = structType->getAlignment();
        }
        else if (static_cast<PointerType*>(mPtr->getType())->getBaseType()->isArrayType())
        {
            ArrayType* arrayType = static_cast<ArrayType*>(static_cast<PointerType*>(mPtr->getType())->getBaseType());
            mType = Type::GetPointerType(arrayType->getBaseType());
            mAlignment = arrayType->getBaseType()->getAlignment();
        }
        else
        {
            mType = mPtr->getType();
            mAlignment = static_cast<PointerType*>(mType)->getBaseType()->getAlignment();
            assert(mType->isPointerType());
        }

        mRequiresRegister = true;
    }
}