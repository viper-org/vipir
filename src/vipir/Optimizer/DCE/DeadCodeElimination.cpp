// Copyright 2024 solar-mist

#include "vipir/Optimizer/DCE/DeadCodeElimination.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/Instruction/Instruction.h"

#include "vipir/Module.h"

namespace vipir
{
    namespace opt
    {
        void DeadCodeEliminator::eliminateDeadCode(Function* function)
        {
            markCriticalInstructions(function);
            sweepInstructions(function);

            while(eliminateUnreachableBlocks(function));
        }


        void DeadCodeEliminator::markCriticalInstructions(Function* function)
        {
            std::vector<Value*> workList;
            for (const auto& basicBlock : function->mBasicBlockList)
            {
                for (const auto& value : basicBlock->mValueList)
                {
                    value->mMarked = false;

                    if (value->isCritical())
                    {
                        value->mMarked = true;
                        workList.push_back(value.get());
                    }
                }
            }

            while (!workList.empty())
            {
                auto front = workList.front();
                workList.erase(workList.begin());

                for (auto operand : front->getOperands())
                {
                    if (!operand->mMarked)
                    {
                        operand->mMarked = true;
                        workList.push_back(operand);
                    }
                }
            }
        }

        void DeadCodeEliminator::sweepInstructions(Function* function)
        {
            for (const auto& basicBlock : function->mBasicBlockList)
            {
                for (size_t i = 0; i < basicBlock->mValueList.size(); ++i)
                {
                    auto& value = basicBlock->mValueList[i];

                    if (!value->mMarked)
                    {
                        for (auto operand : value->getOperands())
                        {
                            if (operand->mMarked)
                            {
                                value->mMarked = true;
                            }
                        }
                        if (!value->mMarked)
                        {
                            if (auto instruction = dynamic_cast<Instruction*>(value.get()))
                            {
                                instruction->eraseFromParent();
                                --i; // Don't increment at the end of the loop as we have just erased
                            }
                        }
                    }
                }
            }
        }


        bool DeadCodeEliminator::eliminateUnreachableBlocks(Function* function)
        {
            bool eliminatedSome = false;
            for (auto it = function->mBasicBlockList.begin() + 1; it < function->mBasicBlockList.end(); ++it)
            {
                auto& basicBlock = *it;

                if (basicBlock->exists() && basicBlock->predecessors().empty())
                {
                    for (auto successor : basicBlock->successors())
                    {
                        std::erase(successor->predecessors(), basicBlock.get());
                    }
                    basicBlock->mExists = false;
                    eliminatedSome = true;
                }
            }
            return eliminatedSome;
        }


        DCEPass::DCEPass()
            : Pass(PassType::DeadCodeElimination)
        {
        }

        void DCEPass::execute(Module& module)
        {
            DeadCodeEliminator dce;
            for (const auto& global : module.getGlobals())
            {
                if (auto function = dynamic_cast<Function*>(global.get()))
                {
                    dce.eliminateDeadCode(function);
                }
            }
        }
    }
}