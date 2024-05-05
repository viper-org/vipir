// Copyright 2024 solar-mist


#include "vipir/Optimizer/RegAlloc/RegAlloc.h"

#include "vipir/IR/Instruction/CallInst.h"

#include <algorithm>
#include <map>
#include <set>

namespace vipir
{
    namespace opt
    {
        void RegAlloc::assignVirtualRegisters(Function* function, abi::ABI* abi)
        {
            setLiveIntervals(function);

            std::vector<int> physicalRegisters = abi->getGeneralPurposeRegisters();

            std::map<int, VReg*> virtualRegs;
            int virtualRegCount = 1;

            int index = 0;
            for (auto physicalReg : physicalRegisters)
            {
                int id = virtualRegCount++;
                function->mVirtualRegs.push_back(std::make_unique<VReg>(id, physicalRegisters[index++]));
                virtualRegs[id] = function->mVirtualRegs.back().get();
            }

            auto getNextFreeVReg = [&virtualRegs, &virtualRegCount, function, abi](){
                if (virtualRegs.empty())
                {
                    function->mVirtualRegs.push_back(std::make_unique<VReg>(virtualRegCount++, abi->getStackOffsetRegister(), 0));
                    return function->mVirtualRegs.back().get();
                }
                VReg* reg = virtualRegs.begin()->second;
                virtualRegs.erase(virtualRegs.begin());
                return reg;
            };

            std::set<Value*, ActiveValueComparator> activeValues;
            auto ExpireOldIntervals = [&activeValues, &virtualRegs](int i){
                std::erase_if(activeValues, [i, &virtualRegs](Value* value){
                    if (value->mInterval.second <= i)
                    {
                        virtualRegs[value->mVReg->getId()] = value->mVReg;
                        return true;
                    }
                    return false;
                });
            };

            setArguments(function, abi, activeValues, virtualRegs);

            for (auto& basicBlock : function->mBasicBlockList)
            {
                for (auto& value : basicBlock->mValueList)
                {
                    ExpireOldIntervals(value->mInterval.first);
                    if (dynamic_cast<CallInst*>(value.get()))
                    {
                        // any active values using a caller-saved value need to be moved
                        std::vector<int> callerSaved = abi->getCallerSavedRegisters();
                        std::vector<VReg*> destroyedRegisters;

                        // erase caller-saved values from current free register pool
                        for (auto it = virtualRegs.begin(); it != virtualRegs.end();)
                        {
                            auto callerSavedIt = std::find(callerSaved.begin(), callerSaved.end(), it->second->mPhysicalRegister);
                            if (callerSavedIt != callerSaved.end())
                            {
                                destroyedRegisters.push_back(it->second);
                                it = virtualRegs.erase(it);
                            }
                            else ++it;
                        }

                        // swap the register of any active values
                        for (auto value : activeValues)
                        {
                            auto callerSavedIt = std::find(callerSaved.begin(), callerSaved.end(), value->mVReg->mPhysicalRegister);
                            if (callerSavedIt != callerSaved.end()) destroyedRegisters.push_back(value->mVReg);
                            value->mVReg = getNextFreeVReg();
                        }

                        // allow all of the caller-saved registers to be reused again
                        for (auto vreg : destroyedRegisters)
                        {
                            virtualRegs[vreg->getId()] = vreg;
                        }
                    }
                    if (value->requiresVReg())
                    {
                        activeValues.insert(value.get());
                        value->mVReg = getNextFreeVReg();
                    }
                }
            }

            setStackOffsets(function, abi);
        }


        void RegAlloc::setLiveIntervals(Function* function)
        {
            int index = 0;
            for (auto& basicBlock : function->mBasicBlockList)
            {
                for (auto& value : basicBlock->mValueList)
                {
                    value->mInterval.first = index;
                    index++;
                }
            }

            for (auto bb = function->mBasicBlockList.rbegin(); bb != function->mBasicBlockList.rend(); ++bb)
            {
                for (auto it = (*bb)->mValueList.rbegin(); it != (*bb)->mValueList.rend(); ++it)
                {
                    auto& value = *it;
                    for (auto operand : value->getOperands())
                    {
                        // If we haven't set the last usage yet, this is it
                        if (operand->mInterval.second == -1)
                        {
                            operand->mInterval.second = index;
                        }
                    }
                    index--;
                }
            }

            // If any values don't have an end interval, set it to the same as the
            // start - they are never used
            for (auto& basicBlock : function->mBasicBlockList)
            {
                for (auto& value : basicBlock->mValueList)
                {
                    if (value->mInterval.second == -1)
                    {
                        value->mInterval.second = value->mInterval.first;
                    }
                }
            }
        }

        void RegAlloc::setStackOffsets(Function* function, abi::ABI* abi)
        {
            // Set the max required size of each virtual register
            for (auto& basicBlock : function->mBasicBlockList)
            {
                for (auto& value : basicBlock->mValueList)
                {
                    if (value->requiresVReg())
                    {
                        auto it = std::find_if(function->mVirtualRegs.begin(), function->mVirtualRegs.end(), [&value](const auto& vreg){
                            return value->mVReg->getId() == vreg->getId();
                        });
                        if (it != function->mVirtualRegs.end())
                        {
                            if (value->getType()->getSizeInBits() > (*it)->mSize)
                            {
                                (*it)->mSize = value->getType()->getSizeInBits();
                            }
                        }
                    }
                }
            }

            // Find all the virtual registers that we need to spill
            std::vector<VReg*> spills;
            for (auto& vreg : function->mVirtualRegs)
            {
                if (vreg->mOnStack)
                {
                    spills.push_back(vreg.get());
                }
            }

            std::sort(spills.begin(), spills.end(), [](VReg* lhs, VReg* rhs) {
                return lhs->mSize > rhs->mSize;
            });

            int offset = 0;
            for (auto vreg : spills)
            {
                offset += vreg->mSize / 8;
                vreg->mStackOffset = offset;
                vreg->mPhysicalRegister = abi->getStackOffsetRegister();
            }

            function->mTotalStackOffset = (offset + 15) & ~15; // Align to 16 bytes
        }


        void RegAlloc::setArguments(Function* function, abi::ABI* abi, std::set<Value*, ActiveValueComparator>& activeValues, std::map<int, VReg*>& virtualRegs)
        {
            int argumentIndex = 0;
            for (auto& argument : function->mArguments)
            {
                auto it = std::find_if(virtualRegs.begin(), virtualRegs.end(), [argumentIndex, abi](const auto& vreg){
                    return vreg.second->mPhysicalRegister == abi->getParameterRegister(argumentIndex);
                });
                argument->mVReg = it->second;
                virtualRegs.erase(it);
                activeValues.insert(argument.get());
            }
        }
    }
}