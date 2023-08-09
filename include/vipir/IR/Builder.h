// Copyright 2023 solar-mist

/*
 * A builder is the object used to create IR objects
 * such as instructions
*/

#ifndef VIPIR_IR_BUILDER_H
#define VIPIR_IR_BUILDER_H 1

namespace vipir
{
    class Value;

    class BasicBlock;

    class RetInst;

    class Builder
    {
    public:
        Builder();

        void setInsertPoint(BasicBlock* newInsertPoint);

        RetInst* CreateRet(Value* returnValue);

    private:
        BasicBlock* mInsertPoint;
    };
}

#endif // VIPIR_IR_BUILDER_H