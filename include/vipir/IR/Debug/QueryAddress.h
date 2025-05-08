// Copyright 2025 solar-mist

/*
 * A dummy instruction to query the current
 * address inside the generated
 * object file
*/

#ifndef VIPIR_IR_DEBUG_QUERY_ADDRESS_H
#define VIPIR_IR_DEBUG_QUERY_ADDRESS_H 1

#include "vipir/IR/Value.h"

namespace vipir
{
    class QueryAddress : public Value
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;

        uint64_t getAddress();

    protected:
        void emit(lir::Builder& builder) override;

    private:
        QueryAddress(Module& module);

        uint64_t mAddress;
    };
}

#endif // VIPIR_IR_DEBUG_QUERY_ADDRESS_H