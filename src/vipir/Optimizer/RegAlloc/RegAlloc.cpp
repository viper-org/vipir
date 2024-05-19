// Copyright 2024 solar-mist


#include "vipir/Optimizer/RegAlloc/RegAlloc.h"

#include "vipir/IR/Instruction/AllocaInst.h"

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

            doRegalloc(function, virtualRegs, virtualRegCount, abi);

            setStackOffsets(function, abi);
        }

        void RegAlloc::doRegalloc(Function* function, std::map<int, VReg*>& virtualRegs, int virtualRegCount, abi::ABI* abi)
        {
            auto createStackVReg = [&virtualRegs, &virtualRegCount, function, abi](){
                function->mVirtualRegs.push_back(std::make_unique<VReg>(virtualRegCount++, abi->getStackOffsetRegister(), 0));
                ++function->mVirtualRegs.back().get()->mUses;
                return function->mVirtualRegs.back().get();
            };

            auto getNextFreeVReg = [&virtualRegs, &virtualRegCount, abi, createStackVReg](bool requireMemory, std::vector<VReg*> disallowed){
                if (virtualRegs.empty())
                {
                    return createStackVReg();
                }
                if (requireMemory)
                {
                    auto it = std::find_if(virtualRegs.begin(), virtualRegs.end(), [](const auto& vreg){
                        return vreg.second->mOnStack;
                    });
                    if (it == virtualRegs.end()) return createStackVReg();
                    ++it->second->mUses;
                    return it->second;
                }
                auto it = std::find_if(virtualRegs.begin(), virtualRegs.end(), [&disallowed](const auto& vreg){
                    return std::find(disallowed.begin(), disallowed.end(), vreg.second) == disallowed.end();
                });
                if (it == virtualRegs.end()) return createStackVReg();
                VReg* reg = it->second;
                virtualRegs.erase(it);
                ++reg->mUses;
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
                    auto smashes = value->getRegisterSmashes();
                    if (!value->mRegisterSmashesDone && !smashes.empty())
                    {
                        value->mRegisterSmashesDone = true;
                        std::vector<VReg*> destroyedRegisters;
                        for (auto smash : smashes)
                        {
                            destroyedRegisters.push_back(function->mVirtualRegs[smash].get());
                        }

                        // Set the disallowed registers for each active value
                        for (auto value : activeValues)
                        {
                            value->mRegisterSmashesDone = false; // Since register configuration changes, we need to recompute smashes
                            std::copy(destroyedRegisters.begin(), destroyedRegisters.end(), std::back_inserter(value->mDisallowedVRegs));
                        }

                        // Allow every virtual reg to be used again
                        std::map<int, VReg*> virtualRegs;
                        for (auto& vreg : function->mVirtualRegs)
                        {
                            virtualRegs[vreg->getId()] = vreg.get();
                        }
                        doRegalloc(function, virtualRegs, virtualRegs.size(), abi);
                        return;
                    }
                    if (value->requiresVReg())
                    {
                        bool requireMemory = false;
                        if (auto alloca = dynamic_cast<AllocaInst*>(value.get()))
                        {
                            if (alloca->mForceMemory) requireMemory = true;
                        }
                        activeValues.insert(value.get());
                        value->mVReg = getNextFreeVReg(requireMemory, value->mDisallowedVRegs);
                    }
                }
            }
        }


        void RegAlloc::setLiveIntervals(Function* function)
        {
            int index = 0;
            for (auto& basicBlock : function->mBasicBlockList)
            {
                basicBlock->mInterval.first = index;
                for (auto& value : basicBlock->mValueList)
                {
                    value->mInterval.first = index;
                    index++;
                }
                basicBlock->mInterval.second = index;
            }

            for (auto it = function->mBasicBlockList.rbegin(); it != function->mBasicBlockList.rend(); ++it)
            {
                auto& bb = *it;

                std::vector<Value*> live;
                for (auto successor : bb->successors())
                {
                    std::copy(successor->liveIn().begin(), successor->liveIn().end(), std::back_inserter(live));
                }

                for (auto value : live)
                {
                    value->mInterval.first = std::min(value->mInterval.first, bb->mInterval.first);
                    value->mInterval.second = std::max(value->mInterval.second, bb->mInterval.second);
                }

                for (auto valueIt = bb->mValueList.rbegin(); valueIt != bb->mValueList.rend(); ++valueIt)
                {
                    auto& value = *valueIt;

                    for (auto operand : value->getOperands())
                    {
                        operand->mInterval.second = std::max(operand->mInterval.second, value->mInterval.first);
                        live.push_back(operand);
                    }

                    live.erase(std::remove_if(live.begin(), live.end(), [&value](auto liveValue){
                        return liveValue == value.get();
                    }), live.end());
                }

                if (bb->loopEnd())
                {
                    for (auto value : live)
                    {
                        value->mInterval.first = std::min(value->mInterval.first, bb->mInterval.first);
                        value->mInterval.second = std::max(value->mInterval.second, bb->loopEnd()->mInterval.second);
                    }
                }

                bb->liveIn() = live;
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
                            return value->mVReg == vreg.get();
                        });
                        if (it != function->mVirtualRegs.end())
                        {
                            int size = value->getType()->getSizeInBits();
                            if (auto alloca = dynamic_cast<AllocaInst*>(value.get()))
                                size = alloca->getAllocatedType()->getSizeInBits();
                            if (size > (*it)->mSize)
                            {
                                (*it)->mSize = size;
                            }
                        }
                    }
                }
            }

            // Find all the virtual registers that we need to spill
            std::vector<VReg*> spills;
            for (auto& vreg : function->mVirtualRegs)
            {
                if (vreg->mOnStack && !vreg->mArgument)
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

            int stackOffset = -0x10;
            for (auto& argument : function->mArguments)
            {
                if (argumentIndex >= abi->getParameterRegisterCount())
                {
                    int id = function->mVirtualRegs.size();
                    function->mVirtualRegs.push_back(std::make_unique<VReg>(id, abi->getStackOffsetRegister()));
                    VReg* vreg = function->mVirtualRegs.back().get();
                    
                    vreg->mOnStack  = true;
                    vreg->mArgument = true;
                    vreg->mStackOffset = stackOffset;
                    stackOffset -= 8;

                    argument->mVReg = vreg;
                    activeValues.insert(argument.get());
                }
                else
                {
                    auto it = std::find_if(virtualRegs.begin(), virtualRegs.end(), [argumentIndex, abi](const auto& vreg){
                        return vreg.second->mPhysicalRegister == abi->getParameterRegister(argumentIndex);
                    });
                    ++argumentIndex;
                    argument->mVReg = it->second;
                    virtualRegs.erase(it);
                    activeValues.insert(argument.get());
                }
            }
        }
    }
}