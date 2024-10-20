// Copyright 2024 solar-mist


#include "vipir/Optimizer/Peephole/PeepholeV2.h"

#include "vipir/LIR/Label.h"
#include "vipir/LIR/Instruction/EnterFunc.h"
#include "vipir/LIR/Instruction/Jump.h"

namespace vipir
{
    namespace opt
    {
        void PeepholeV2::doPeephole(lir::Builder& builder)
        {
            bool hasOptimized = false;
            auto& instructions = builder.getValues();

            for (auto it = instructions.begin()+1; it != instructions.end();)
            {
                bool iterate = true;
                auto unreachableLabel = checkUnreachableLabel(builder, *it);
                if (unreachableLabel == UnreachableLabelLevel::Unreachable && dynamic_cast<lir::EnterFunc*>((it - 1)->get()))
                {
                    unreachableLabel = UnreachableLabelLevel::OptimizedReferences; // Don't entirely remove the entry BB of a function
                }
                if (unreachableLabel != UnreachableLabelLevel::References)
                {
                    it = eraseValue(instructions, it);
                    if (unreachableLabel == UnreachableLabelLevel::Unreachable)
                    {
                        // Remove all LIR nodes contained inside the label
                        while (!dynamic_cast<lir::Label*>(it->get()))
                        {
                            it = eraseValue(instructions, it);
                        }
                    }

                    iterate = false;
                    hasOptimized = true;
                }

                if (it - 1 < instructions.end())
                {
                    if (checkJmpLabel(*it, *(it + 1)))
                    {
                        it = eraseValue(instructions, it);
                        iterate = false;
                        hasOptimized = true;
                    }
                }
                if (iterate) ++it;
            }

            if (hasOptimized) doPeephole(builder); // Repeat until no more optimizations are available
        }

        std::vector<lir::ValuePtr>::iterator PeepholeV2::eraseValue(std::vector<lir::ValuePtr>& values, std::vector<lir::ValuePtr>::iterator it)
        {
            auto& value = *it;

            for (auto& input : value->getInputOperands())
            {
                if (auto vreg = dynamic_cast<lir::VirtualReg*>(input.get().get()))
                {
                    vreg->mVreg->mUses--;
                }
            }

            for (auto& output : value->getOutputOperands())
            {
                if (auto vreg = dynamic_cast<lir::VirtualReg*>(output.get().get()))
                {
                    vreg->mVreg->mUses--;
                }
            }

            return values.erase(it);
        }



        PeepholeV2::UnreachableLabelLevel PeepholeV2::checkUnreachableLabel(lir::Builder& builder, lir::ValuePtr& value)
        {
            auto label = dynamic_cast<lir::Label*>(value.get());
            if (!label) return UnreachableLabelLevel::References;
            if (label->mGlobal) return UnreachableLabelLevel::References;

            for (const auto& value : builder.getValues())
            {
                for (const auto& operand : value->getInputOperands())
                {
                    if (auto labelOperand = dynamic_cast<lir::Lbl*>(operand.get().get()))
                    {
                        if (labelOperand->mName == label->mName)
                        {
                            return UnreachableLabelLevel::References;
                        }
                    }
                }
            }
            if (label->mHadReferenceOptimized) return UnreachableLabelLevel::OptimizedReferences;
            return UnreachableLabelLevel::Unreachable;
        }

        bool PeepholeV2::checkJmpLabel(lir::ValuePtr& first, lir::ValuePtr& second)
        {
            auto jmp = dynamic_cast<lir::Jump*>(first.get());
            if (!jmp) return false;

            auto label = dynamic_cast<lir::Label*>(second.get());
            if (!label) return false;

            for (const auto& operand : jmp->getInputOperands())
            {
                if (auto labelOperand = dynamic_cast<lir::Lbl*>(operand.get().get()))
                {
                    if (labelOperand->mName == label->mName)
                    {
                        label->mHadReferenceOptimized = true;
                        return true;
                    }
                }
            }
            
            return false;
        }
    }
}