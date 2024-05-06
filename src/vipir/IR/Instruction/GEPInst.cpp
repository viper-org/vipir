// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/GEPInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/Type/ArrayType.h"
#include "vipir/Type/PointerType.h"
#include "vipir/Type/StructType.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/LoadAddress.h"

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
        stream << std::format("gep %{}, {}, {}", getName(mValueId), mPtr->ident(), mOffset->ident());
    }

    std::string GEPInst::ident() const
    {
        return std::format("%{}", getName(mValueId));
    }

    std::vector<Value*> GEPInst::getOperands()
    {
        return std::vector<Value*>{mPtr, mOffset};
    }

    void GEPInst::emit(MC::Builder& builder)
    {
        codegen::OperandSize size = mType->getOperandSize();
        //instruction::OperandPtr operand = mVReg->operand(size);

        instruction::OperandPtr ptr = mPtr->getEmittedValue();
        instruction::OperandPtr offset = mOffset->getEmittedValue();

        instruction::Register* ptrReg = dynamic_cast<instruction::Register*>(ptr.get());
        instruction::Memory* ptrMem = dynamic_cast<instruction::Memory*>(ptr.get());
        instruction::LabelOperand* ptrLabel = dynamic_cast<instruction::LabelOperand*>(ptr.get());
        instruction::Relative* ptrRel = dynamic_cast<instruction::Relative*>(ptr.get());
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
            else if (ptrRel)
            {
                auto label = ptrRel->getLabel();
                disp += *ptrRel->getDisplacement();
                (void)ptr.release();
                memory = std::make_unique<instruction::Relative>(instruction::LabelOperandPtr(label), disp);
            }
            else
            {
                (void)ptr.release();
                memory = std::make_unique<instruction::Memory>(instruction::RegisterPtr(ptrReg), disp, nullptr, std::nullopt);
            }
            //builder.addValue(std::make_unique<instruction::LeaInstruction>(operand->clone(), std::move(memory)));
            mEmittedValue = std::move(memory);
        }
        else if (auto regOffset = dynamic_cast<instruction::Register*>(offset.get()))
        {
            (void)ptr.release();
            instruction::Register* index = regOffset;
            (void)offset.release();

            instruction::OperandPtr memory = std::make_unique<instruction::Memory>(instruction::RegisterPtr(ptrReg), displacement, instruction::RegisterPtr(regOffset), scale);
            //builder.addValue(std::make_unique<instruction::LeaInstruction>(operand->clone(), std::move(memory)));
            mEmittedValue = std::move(memory);
        }
    }

    void GEPInst::emit2(lir::Builder& builder)
    {
        mPtr->lateEmit(builder);
        mOffset->lateEmit(builder);

        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
        lir::OperandPtr ptr = mPtr->getEmittedValue2();
        lir::OperandPtr offset = mOffset->getEmittedValue2();

        int scale = mAlignment / 8;

        if (StructType* structType = dynamic_cast<StructType*>(static_cast<PointerType*>(mPtr->getType())->getBaseType()))
        {
            lir::Immediate* immediate = static_cast<lir::Immediate*>(offset.get());
            int disp = 0;
            for (int i = 0; i < immediate->value(); ++i)
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

            offset = std::make_unique<lir::Immediate>(disp);
            scale = 1;
        }

        lir::OperandPtr mem = std::make_unique<lir::Memory>(std::move(ptr), std::nullopt, std::move(offset), scale);
        builder.addValue(std::make_unique<lir::LoadAddress>(vreg->clone(), std::move(mem)));
        mEmittedValue2 = std::make_unique<lir::Memory>(std::move(vreg), std::nullopt, nullptr, std::nullopt);
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
        //mRequiresVReg = false;
    }
}