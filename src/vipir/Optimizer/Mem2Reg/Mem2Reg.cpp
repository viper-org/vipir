// Copyright 2024 solar-mist

#include "vipir/Optimizer/Mem2Reg/Mem2Reg.h"

#include "vipir/Module.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/Instruction/StoreInst.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/PhiInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/BranchInst.h"

#include <algorithm>

namespace vipir
{
    namespace opt
    {
        void Mem2Reg::doMem2Reg(Function* function)
        {
            if (function->mBasicBlockList.empty()) return;
    
            DominatorAnalyzer dom;
            dom.computeDominanceFrontiers(function);
            auto wherePhi = phiInsertPosition(function);
            insertPhi(wherePhi);
            decideValuesStartFrom(function, function->mBasicBlockList.front().get(), {}, {});
        }

        std::map<AllocaInst*, std::set<BasicBlock*> > Mem2Reg::phiInsertPosition(Function* function)
        {
            std::map<AllocaInst*, std::set<BasicBlock*> > result;

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

        void Mem2Reg::insertPhi(std::map<AllocaInst*, std::set<BasicBlock*> > where)
        {
            for (auto [var, bbs] : where)
            {
                for (auto bb : bbs)
                {
                    auto phi = new PhiInst(bb, var->getAllocatedType(), var);
                    bb->mValueList.insert(bb->mValueList.begin(), std::unique_ptr<PhiInst>(phi));
                }
            }
        }

        std::pair<BasicBlock*, Value*> Mem2Reg::decideVariableValue(AllocaInst* var,
            std::deque<std::map<AllocaInst*, std::pair<BasicBlock*, Value*> > > current)
        {
            for (auto it = current.begin(); it != current.end(); ++it)
            {
                auto frame = *it;
                if (frame.find(var) != frame.end()) return frame[var];
            }
            throw nullptr;
        }
        
        void Mem2Reg::decideValuesStartFrom(Function* function, BasicBlock* basicBlock, std::set<BasicBlock*> visited,
            std::deque<std::map<AllocaInst*, std::pair<BasicBlock*, Value*> > > current)
        {
            for (auto& value : basicBlock->mValueList)
            {
                if (auto phi = dynamic_cast<PhiInst*>(value.get()))
                {
                    if (phi->mAlloca) // Only check phis that were created by mem2reg
                    {
                        auto [currentFrom, currentValue] = decideVariableValue(phi->mAlloca, current);
                        phi->addIncoming(currentValue, currentFrom);
                        current.front()[phi->mAlloca] = std::make_pair(basicBlock, phi);
                    }
                }
            }

            if (visited.contains(basicBlock)) return;
            visited.insert(basicBlock);

            for (auto it = basicBlock->mValueList.begin(); it != basicBlock->mValueList.end();)
            {
                auto& value = *it;
                bool changed = false;
                if (auto load = dynamic_cast<LoadInst*>(value.get()))
                {
                    changed = true;

                    auto ptr = load->getPointer();
                    if (auto alloca = dynamic_cast<AllocaInst*>(ptr))
                    {
                        if (alloca->forceMemoryCount()) continue;
                    }
                    else continue;
                    auto [_, loadValue] = decideVariableValue(static_cast<AllocaInst*>(ptr), current);
                    replace(function, load, loadValue);
                    load->eraseFromParent();
                }
                if (auto store = dynamic_cast<StoreInst*>(value.get()))
                {
                    changed = true;
                    auto value = store->mValue;
                    auto ptr = store->mPtr;
                    if (auto alloca = dynamic_cast<AllocaInst*>(ptr))
                    {
                        if (alloca->forceMemoryCount()) continue;
                    }
                    else continue;
                    if (current.empty()) current.push_back(std::map<AllocaInst*, std::pair<BasicBlock*, Value*> >());
                    current.back()[static_cast<AllocaInst*>(ptr)] = std::make_pair(basicBlock, value);
                    store->eraseFromParent();
                }
                if (auto branch = dynamic_cast<BranchInst*>(value.get()))
                {
                    if (branch->mCondition)
                    {
                        current.push_front(std::map<AllocaInst*, std::pair<BasicBlock*, Value*> >());
                        decideValuesStartFrom(function, branch->mTrueBranch, visited, current);
                        current.pop_front();

                        current.push_front(std::map<AllocaInst*, std::pair<BasicBlock*, Value*> >());
                        decideValuesStartFrom(function, branch->mFalseBranch, visited, current);
                        current.pop_front();
                    }
                    else
                    {
                        decideValuesStartFrom(function, branch->mTrueBranch, visited, current);
                    }
                }
                
                if (!changed) ++it;
            }
        }

        void Mem2Reg::replace(Function* function, Value* old, Value* newValue)
        {
            for (auto& basicBlock : function->mBasicBlockList)
            {
                for (auto& value : basicBlock->mValueList)
                {
                    for (auto operandR : value->getOperands())
                    {
                        auto& operand = operandR.get();
                        if (operand == old)
                        {
                            operand = newValue;
                        }
                    }
                }
            }
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