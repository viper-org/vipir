// Copyright 2024 solar-mist

#include "vipir/Optimizer/Mem2Reg/Dom.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/BasicBlock.h"

namespace vipir
{
    namespace opt
    {
        BasicBlock* DominatorAnalyzer::Intersect(BasicBlock* b1, BasicBlock* b2)
        {
            auto finger1 = b1;
            auto finger2 = b2;
            while (finger1 != finger2)
            {
                while (finger1->mIndexInFunction < finger2->mIndexInFunction)
                {
                    finger1 = finger1->mDom;
                }
                while (finger2->mIndexInFunction < finger1->mIndexInFunction)
                {
                    finger2 = finger2->mDom;
                }
            }
            return finger1;
        }

        void DominatorAnalyzer::computeDominanceFrontiers(Function* function)
        {
            bool changed = true;
            function->mBasicBlockList.front()->mDom = function->mBasicBlockList.front().get();
            function->mBasicBlockList.front()->mDomCalculated = true;
            while (changed)
            {
                changed = false;
                for (auto it = function->mBasicBlockList.begin() + 1; it != function->mBasicBlockList.end(); ++it)
                {
                    auto& basicBlock = *it;
                    BasicBlock* newIdom = nullptr;
                    bool foundFirst = false;
                    for (auto pIt = basicBlock->predecessors().begin() + 1; pIt < basicBlock->predecessors().end(); ++pIt)
                    {
                        auto predecessor = *pIt;
                        if (predecessor->mDomCalculated) 
                        {
                            if (foundFirst)
                                newIdom = Intersect(predecessor, newIdom);
                            else newIdom = predecessor;
                        }
                    }
                    if (basicBlock->mDom != newIdom)
                    {
                        basicBlock->mDom = newIdom;
                        basicBlock->mDomCalculated = true;
                        changed = true;
                    }
                }
            }

            for (auto& basicBlock : function->mBasicBlockList)
            {
                if (basicBlock->predecessors().size() >= 2)
                {
                    for (auto predecessor : basicBlock->predecessors())
                    {
                        auto runner = predecessor;
                        while (runner != basicBlock->mDom)
                        {
                            runner->mDominanceFrontier.push_back(basicBlock.get());
                            runner = runner->mDom;
                        }
                    }
                }
            }
        }
    }
}