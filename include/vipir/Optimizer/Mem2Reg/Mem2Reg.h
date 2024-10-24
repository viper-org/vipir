// Copyright 2024 solar-mist

#ifndef VIPIR_OPTIMIZER_MEM2REG_MEM2REG_H
#define VIPIR_OPTIMIZER_MEM2REG_MEM2REG_H 1

#include "vipir/Optimizer/Mem2Reg/Dom.h"

#include "vipir/Pass/Pass.h"

#include <set>
#include <deque>
#include <unordered_map>

namespace vipir
{
    class Function;
    class BasicBlock;
    class Value;
    class AllocaInst;

    namespace opt
    {
        class Mem2Reg
        {
        public:
            void doMem2Reg(Function* function);

        private:
            std::unordered_map<AllocaInst*, std::set<BasicBlock*> > phiInsertPosition(Function* function);
            void insertPhi(std::unordered_map<AllocaInst*, std::set<BasicBlock*> > where);

            std::pair<BasicBlock*, Value*> decideVariableValue(AllocaInst* var,
                std::deque<std::unordered_map<AllocaInst*, std::pair<BasicBlock*, Value*> > > current);
            void decideValuesStartFrom(Function* function, BasicBlock* basicBlock, std::set<BasicBlock*> visited,
                std::deque<std::unordered_map<AllocaInst*, std::pair<BasicBlock*, Value*> > > current);

            void replace(Function* function, Value* old, Value* newValue);
        };

        class Mem2RegPass : public Pass
        {
        public:
            Mem2RegPass();

            virtual void execute(Module& module) override;
        };
    }
}

#endif // VIPIR_OPTIMIZER_MEM2REG_MEM2REG_H