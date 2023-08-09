// Copyright 2023 solar-mist


#include "vipir/Type/IntegerType.h"

namespace vipir
{
    IntegerType::IntegerType(int bits)
        : Type(bits, "i" + std::to_string(bits))
    {
    }
}