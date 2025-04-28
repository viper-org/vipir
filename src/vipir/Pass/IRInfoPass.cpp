// Copyright 2025 solar-mist

#include "vipir/Pass/IRInfoPass.h"

#include "vipir/Module.h"

#include "vipir/IR/Function.h"

#include <fstream>
#include <format>

namespace vipir
{
    CFGEmissionPass::CFGEmissionPass(std::string outpath)
        : Pass(PassType::CFGEmission)
        , mOutPath(std::move(outpath))
    {
    }

    
    void CFGEmissionPass::execute(Module& module)
    {
        std::ofstream outFile(mOutPath);
        outFile << "digraph G {\n";
        for (auto& global : module.getGlobals())
        {
            if (auto function = dynamic_cast<Function*>(global.get()))
            {
                if (function->getBasicBlocks().empty()) continue;
                std::vector<BasicBlock*> visited;
                for (auto& bb : function->getBasicBlocks())
                {
                    for (auto pred : bb->predecessors())
                    {
                        outFile << std::format("  {} -> {};\n", pred->ident().substr(8), bb->ident().substr(8));
                    }
                }
            }
        }
        outFile << "}\n";
    }
}