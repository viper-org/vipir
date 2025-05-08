// Copyright 2025 solar-mist

/*
 * A dummy instruction to tell the compiler
 * to emit a source location
*/

#ifndef VIPIR_IR_DEBUG_EMIT_DEBUG_INFO_H
#define VIPIR_IR_DEBUG_EMIT_DEBUG_INFO_H 1

#include "vipir/IR/Value.h"

namespace vipir
{
    class EmitDebugInfo : public Value
    {
    friend class IRBuilder;
    public:
        void print(std::ostream& stream) override;
        std::string ident() const override;

        void doConstantFold() override;

    protected:
        void emit(lir::Builder& builder) override;

    private:
        EmitDebugInfo(Module& module, int line, int col);

        int mLine;
        int mCol;
    };
}

#endif // VIPIR_IR_DEBUG_EMIT_DEBUG_INFO_H