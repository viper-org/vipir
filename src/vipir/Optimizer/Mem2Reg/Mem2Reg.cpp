// Copyright 2024 solar-mist

#include "vipir/Optimizer/Mem2Reg/Mem2Reg.h"

#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"

#include <algorithm>

namespace vipir
{
    namespace opt
    {
        void Mem2Reg::doMem2Reg(Function* function)
        {
            DominatorAnalyzer dom;
            dom.computeDominanceFrontiers(function);
            phiInsertPosition(function);
        }

        std::unordered_map<AllocaInst*, std::set<BasicBlock*> > Mem2Reg::phiInsertPosition(Function* function)
        {
            std::unordered_map<AllocaInst*, std::set<BasicBlock*> > result;

            for (auto variable : function->getAllocaList())
            {
                std::vector<BasicBlock*> blocksContainStore;
                for (auto& block : function->mBasicBlockList)
                {
                    for (auto& value : block->mValueList)
                    {
                        if (auto store = dynamic_cast<StoreInst*>(value.get()))
                        {
                            if (store->mPtr == variable)
                            {
                                blocksContainStore.push_back(block.get());
                            }
                        }
                    }
                }

                auto blocksToConsider = blocksContainStore;
                while(!blocksToConsider.empty())
                {
                    auto consideringBlock = blocksToConsider.back();
                    blocksToConsider.pop_back();
                    for (auto dominatorFrontier : consideringBlock->mDominanceFrontier)
                    {
                        result[variable].insert(dominatorFrontier);
                        if (std::find(blocksContainStore.begin(), blocksContainStore.end(), dominatorFrontier)
                            == blocksContainStore.end())
                        {
                            blocksToConsider.push_back(dominatorFrontier);
                        }
                    }
                }
            }
            return result;
        }


        Mem2RegPass::Mem2RegPass()
            : Pass(PassType::Mem2Reg)
        {
        }

        void Mem2RegPass::execute(Module& module)
        {
            Mem2Reg mem2reg;
            for (const auto& global : module.getGlobals())
            {
                if (auto function = dynamic_cast<Function*>(global.get()))
                {
                    mem2reg.doMem2Reg(function);
                }
            }
        }
    }
}