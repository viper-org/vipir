// Copyright 2024 solar-mist

#include "vipir/Optimizer/DCE/DeadCodeElimination.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/Instruction/Instruction.h"

namespace vipir
{
    namespace opt
    {
        void DeadCodeEliminator::eliminateDeadCode(Function* function)
        {
            markCriticalInstructions(function);
            sweepInstructions(function);
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
                    for (auto operand : value->getOperands())
                    {
                        if (operand->isCritical() && !operand->mMarked)
                        {
                            operand->mMarked = true;
                            workList.push_back(value.get());
                        }
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
}