// Copyright 2023 solar-mist

/*
 * A builder is the object used to create IR objects
 * such as instructions
*/

#ifndef VIPIR_IR_BUILDER_H
#define VIPIR_IR_BUILDER_H 1

#include "vipir/IR/Value.h"

#include <cstdint>

namespace vipir
{
    class BasicBlock;

    class RetInst;
    class AllocaInst;

    class ConstantInt;

    class Builder
    {
    public:
        Builder();

        void setInsertPoint(BasicBlock* newInsertPoint);

        RetInst* CreateRet(Value* returnValue);

        AllocaInst* CreateAlloca(Type* allocatedType, std::string name = "");

        ConstantInt* CreateConstantInt(uint64_t value, Type* type, std::string name = "");

    private:
        BasicBlock* mInsertPoint;
    };
}

#endif // VIPIR_IR_BUILDER_H