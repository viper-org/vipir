// Copyright 2025 solar-mist


#include "vipir/IR/Debug/QueryAddress.h"

namespace vipir
{
    void QueryAddress::print(std::ostream& stream)
    {
        stream << std::format(".loc {}:{}", mLine, mCol);
    }

    std::string QueryAddress::ident() const
    {
        return "%undef";
    }

    void QueryAddress::doConstantFold()
    {
    }

    uint64_t QueryAddress::getAddress()
    {
        return mAddress;
    }


    void QueryAddress::emit(lir::Builder& builder)
    {
        builder.addValue(std::make_unique<lir::QueryAddress>(&mAddress));
    }

    QueryAddress::QueryAddress(Module& module)
        : Value(module)
    {
    }
}