// Copyright 2024 solar-mist

/*
 * Passes for common compilation stages e.g.
 * codegen
*/

#ifndef VIPIR_PASS_DEFAULT_PASS_H
#define VIPIR_PASS_DEFAULT_PASS_H 1

#include "vipir/Pass/Pass.h"

namespace vipir
{
    class DIBuilder;

    class LIREmissionPass : public Pass
    {
    public:
        LIREmissionPass();

        virtual void execute(Module& module) override;
    };

    class LIRCodegenPass : public Pass
    {
    public:
        LIRCodegenPass();

        virtual void execute(Module& module) override;
    };

    class DebugInfoEmissionPass : public Pass
    {
    public:
        DebugInfoEmissionPass(DIBuilder* builder);

        virtual void execute(Module& module) override;

    private:
        DIBuilder* mBuilder;
    };


    class ConstantFoldingPass : public Pass
    {
    public:
        ConstantFoldingPass();

        virtual void execute(Module& module) override;
    };
}

#endif // VIPIR_PASS_DEFAULT_PASS_H