// Copyright 2024 solar-mist


#include "vipir/IR/Instruction/GEPInst.h"

#include "vipir/IR/BasicBlock.h"

#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/Type/ArrayType.h"
#include "vipir/Type/PointerType.h"
#include "vipir/Type/StructType.h"

#include "vipir/Module.h"

#include "vipir/LIR/Instruction/LoadAddress.h"
#include "vipir/LIR/Instruction/Arithmetic.h"
#include "vipir/LIR/Instruction/Move.h"

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
        stream << std::format("%{} = gep {}, {}", getName(mValueId), mPtr->ident(), mOffset->ident());
    }

    std::string GEPInst::ident() const
    {
        return std::format("%{}", getName(mValueId));
    }

    void GEPInst::doConstantFold()
    {
    }

    std::vector<std::reference_wrapper<Value*> > GEPInst::getOperands()
    {
        return {mPtr, mOffset};
    }

    constexpr std::size_t AlignUp(std::size_t value, std::size_t align)
    {
        return (value + align - 1) & ~(align - 1);
    }

    void GEPInst::emit(lir::Builder& builder)
    {
        mPtr->lateEmit(builder);
        mOffset->lateEmit(builder);

        lir::OperandPtr vreg = std::make_unique<lir::VirtualReg>(mVReg, mType->getOperandSize());
        lir::OperandPtr ptr = mPtr->getEmittedValue();
        lir::OperandPtr offset = mOffset->getEmittedValue();

        int scale = mAlignment / 8;
        
        std::optional<int> displacement;
        if (StructType* structType = dynamic_cast<StructType*>(static_cast<PointerType*>(mPtr->getType())->getBaseType()))
        {
            lir::Immediate* immediate = static_cast<lir::Immediate*>(offset.get());
            displacement = 0;
            for (int i = 0; i < immediate->value(); ++i)
            {
                if (structType->getField(i)->isArrayType())
                {
                    ArrayType* arrayType = static_cast<ArrayType*>(structType->getField(i));
                    
                    *displacement += AlignUp(arrayType->getSizeInBits() / 8, (std::size_t)scale);
                }
                else
                {
                    *displacement += scale;
                }
            }

            offset = nullptr;
            scale = 0;
            lir::OperandPtr mem = std::make_unique<lir::Memory>(mType->getOperandSize(), std::move(ptr), displacement, std::move(offset), scale);
            mEmittedValue = std::move(mem);
            return;
        }

        if (PointerType* pointerType = dynamic_cast<PointerType*>(static_cast<PointerType*>(mPtr->getType())->getBaseType()))
        {
            if (StructType* structType = dynamic_cast<StructType*>(pointerType->getBaseType()))
            {
                scale = 1;
                lir::OperandPtr scale = std::make_unique<lir::Immediate>(structType->getSizeInBits() / 8);
                builder.addValue(std::make_unique<lir::Move>(vreg->clone(), std::move(offset)));
                builder.addValue(std::make_unique<lir::BinaryArithmetic>(vreg->clone(), lir::BinaryArithmetic::Operator::IMul, std::move(scale)));
                offset = vreg->clone();
            }
        }

        codegen::OperandSize size = static_cast<PointerType*>(mType)->getBaseType()->getOperandSize();
        
        lir::OperandPtr mem = std::make_unique<lir::Memory>(mType->getOperandSize(), std::move(ptr), displacement, std::move(offset), scale);
        mEmittedValue = std::move(mem);
        return;
        builder.addValue(std::make_unique<lir::LoadAddress>(vreg->clone(), std::move(mem)));
        mEmittedValue = std::move(vreg);
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