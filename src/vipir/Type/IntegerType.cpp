// Copyright 2024 solar-mist

#include "vipir/Type/IntegerType.h"

#include <format>

namespace vipir
{
    IntegerType::IntegerType(int bits)
        : Type(bits, std::format("i{}", bits))
    {
        switch(bits)
        {
            case 64:
                mOperandSize = codegen::OperandSize::Quad;
                break;
            case 32:
                mOperandSize = codegen::OperandSize::Long;
                break;
            case 16:
                mOperandSize = codegen::OperandSize::Word;
                break;
            case 8:
                mOperandSize = codegen::OperandSize::Byte;
                break;
            default:
                break;
        }
    }
}