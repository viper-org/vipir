// Copyright 2024 solar-mist

#include "vipir/Pass/DefaultPass.h"

#include "vipir/Module.h"

namespace vipir
{
    LIREmissionPass::LIREmissionPass()
        : Pass(PassType::LIREmission)
    {
    }

    void LIREmissionPass::execute(Module& module)
    {
        module.lirEmission();
    }



    LIRCodegenPass::LIRCodegenPass()
        : Pass(PassType::LIRCodegen)
    {
    }

    void LIRCodegenPass::execute(Module& module)
    {
        module.lirCodegen();
    }


    ConstantFoldingPass::ConstantFoldingPass()
        : Pass(PassType::ConstantFolding)
    {
    }

    void ConstantFoldingPass::execute(Module& module)
    {
        for (auto& constant : module.getConstants())
        {
            constant->doConstantFold();
        }
        for (auto& global : module.getGlobals())
        {
            global->doConstantFold();
        }
    }
}