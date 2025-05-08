// Copyright 2024 solar-mist


#include "vipir/Optimizer/RegAlloc/RegAlloc.h"

#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/PhiInst.h"
#include "vipir/Type/StructType.h"

#include <algorithm>
#include <map>

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
            for (auto physicalReg: physicalRegisters)
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
            auto createStackVReg = [&virtualRegs, &virtualRegCount, function, abi]() {
                function->mVirtualRegs.push_back(std::make_unique<VReg>(virtualRegCount++, abi->getStackOffsetRegister(), 0));
                ++function->mVirtualRegs.back().get()->mUses;
                return function->mVirtualRegs.back().get();
            };

            auto getNextFreeVReg = [&virtualRegs, &virtualRegCount, abi, createStackVReg](bool requireMemory, std::vector<VReg*> disallowed, int preferred) {
                if (virtualRegs.empty())
                {
                    return createStackVReg();
                }
                if (requireMemory)
                {
                    auto it = std::find_if(virtualRegs.begin(), virtualRegs.end(), [](const auto& vreg) {
                        return vreg.second->mOnStack;
                    });
                    if (it == virtualRegs.end()) return createStackVReg();
                    ++it->second->mUses;
                    VReg* vreg = it->second;
                    virtualRegs.erase(it);
                    return vreg;
                }
                if (preferred != -1)
                {
                    auto it = std::find_if(virtualRegs.begin(), virtualRegs.end(), [preferred, &disallowed](const auto& vreg) {
                        return std::find(disallowed.begin(), disallowed.end(), vreg.second) == disallowed.end() && vreg.second->getPhysicalRegister() == preferred;
                    });
                    if (it != virtualRegs.end())
                    {
                        VReg* reg = it->second;
                        virtualRegs.erase(it);
                        ++reg->mUses;
                        return reg;
                    }
                }
                auto it = std::find_if(virtualRegs.begin(), virtualRegs.end(), [&disallowed](const auto& vreg) {
                    return std::find(disallowed.begin(), disallowed.end(), vreg.second) == disallowed.end();
                });
                if (it == virtualRegs.end()) return createStackVReg();
                VReg* reg = it->second;
                virtualRegs.erase(it);
                ++reg->mUses;
                return reg;
            };

            std::vector<Value*> activeValues;
            auto ExpireOldIntervals = [&activeValues, &virtualRegs](int i) {
                std::erase_if(activeValues, [i, &virtualRegs](Value* value) {
                    if (value->mInterval.second <= i)
                    {
                        virtualRegs[value->mVReg->getId()] = value->mVReg;
                        return true;
                    }
                    return false;
                });
            };

            auto arguments = setArguments(function, abi, activeValues, virtualRegs);
            for (auto argument : arguments)
            {
                argument->mMoveTo = getNextFreeVReg(false, argument->mDisallowedVRegs, -1);
            }

            std::sort(activeValues.begin(), activeValues.end(), [](Value* a, Value* b) {
                return a->mInterval.second < b->mInterval.second;
            });

            //std::vector<Value*> values;
            struct Compare
            {
                bool operator()(Value* a, Value* b) const
                {
                    return a->mInterval.first < b->mInterval.first;
                }
            };
            std::multiset<Value*, Compare> values;
            for (auto& basicBlock: function->mBasicBlockList)
            {
                for (auto& value : basicBlock->mValueList)
                {
                    if (dynamic_cast<PhiInst*>(value.get()))
                    {
                        auto guy = 5;
                    }
                    values.insert(value.get());
                }
            }
            for (auto value: values)
            {
                ExpireOldIntervals(value->mId);
                auto smashes = value->getRegisterSmashes();
                if (!value->mRegisterSmashesDone && !smashes.empty())
                {
                    value->mRegisterSmashesDone = true;
                    std::vector<VReg*> destroyedRegisters;
                    for (auto smash: smashes)
                    {
                        auto it = std::find_if(function->mVirtualRegs.begin(), function->mVirtualRegs.end(), [smash](const auto& vreg) {
                            return vreg->mPhysicalRegister == smash;
                        });

                        if (it != function->mVirtualRegs.end())
                            destroyedRegisters.push_back(it->get());
                    }

                    // Set the disallowed registers for each active value
                    for (auto value: activeValues)
                    {
                        value->mRegisterSmashesDone = false; // Since register configuration changes, we need to recompute smashes
                        std::copy(destroyedRegisters.begin(), destroyedRegisters.end(), std::back_inserter(value->mDisallowedVRegs));
                    }

                    // Allow every virtual reg to be used again
                    std::map<int, VReg*> virtualRegs;
                    for (auto& vreg: function->mVirtualRegs)
                    {
                        vreg->mUses = 0;
                        virtualRegs[vreg->getId()] = vreg.get();
                    }
                    doRegalloc(function, virtualRegs, virtualRegs.size(), abi);
                    return;
                }
                if (value->requiresVReg())
                {
                    bool requireMemory = false;
                    if (auto alloca = dynamic_cast<AllocaInst*>(value))
                    {
                        if (alloca->mForceMemoryCount > 0) requireMemory = true;
                    }
                    activeValues.push_back(value);
                    std::sort(activeValues.begin(), activeValues.end(), [](Value* a, Value* b) {
                        return a->mInterval.second < b->mInterval.second;
                    });
                    value->mVReg = getNextFreeVReg(requireMemory, value->mDisallowedVRegs, value->mPreferredRegisterID);
                }
            }
        }


        void RegAlloc::setLiveIntervals(Function* function)
        {
            int index = 0;

            for (auto& argument : function->mArguments)
            {
                argument->mInterval.first = 0;
            }

            for (auto& basicBlock: function->mBasicBlockList)
            {
                basicBlock->mInterval.first = index;
                for (auto& value: basicBlock->mValueList)
                {
                    value->mInterval.first = index;
                    //value->mInterval.second = index+1;
                    value->mId = index;
                    index++;
                }
                basicBlock->mInterval.second = index;
            }

            for (auto it = function->mBasicBlockList.rbegin(); it != function->mBasicBlockList.rend(); ++it)
            {
                auto& bb = *it;
                std::vector<Value*> live;

                for (auto successor: bb->successors())
                {
                    std::copy(successor->liveIn().begin(), successor->liveIn().end(), std::back_inserter(live));

                    for (auto phi: successor->mPhis)
                    {
                        auto incomingIt = std::find_if(phi->mIncoming.begin(), phi->mIncoming.end(), [&bb](const auto& incoming) {
                            return incoming.second == bb.get();
                        });

                        if (incomingIt != phi->mIncoming.end())
                        {
                            live.push_back(incomingIt->first);
                        }
                        //live.push_back(phi);
                    }
                }

                for (auto value: live)
                {
                    value->mInterval.first = std::min(value->mInterval.first, bb->mInterval.first);
                    value->mInterval.second = std::max(value->mInterval.second, bb->mInterval.second);
                }

                for (auto valueIt = bb->mValueList.rbegin(); valueIt != bb->mValueList.rend(); ++valueIt)
                {
                    auto& value = *valueIt;

                    for (auto operandR: value->getOperands())
                    {
                        auto operand = operandR.get();
                        operand->mInterval.first = std::min(operand->mInterval.first, bb->mInterval.first);
                        operand->mInterval.second = std::max(operand->mInterval.second, value->mId);
                        live.push_back(operand);
                    }
                    value->mInterval.first = value->mId;

                    live.erase(std::remove_if(live.begin(), live.end(), [&value](auto liveValue) {
                        return liveValue == value.get();
                    }), live.end());
                }

                for (auto phi: bb->mPhis)
                {
                    live.erase(std::remove(live.begin(), live.end(), phi), live.end());
                }

                if (bb->loopEnd())
                {
                    for (auto value: live)
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
            for (auto& basicBlock: function->mBasicBlockList)
            {
                for (auto& value: basicBlock->mValueList)
                {
                    if (value->requiresVReg())
                    {
                        auto it = std::find_if(function->mVirtualRegs.begin(), function->mVirtualRegs.end(), [&value](const auto& vreg) {
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
            for (auto& vreg: function->mVirtualRegs)
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
            for (auto vreg: spills)
            {
                offset += vreg->mSize / 8;
                vreg->mStackOffset = offset;
                vreg->mPhysicalRegister = abi->getStackOffsetRegister();
            }

            function->mTotalStackOffset = (offset + 15) & ~15; // Align to 16 bytes
        }

        std::vector<Argument*> RegAlloc::setArguments(Function* function, abi::ABI* abi, std::vector<Value*>& activeValues, std::map<int, VReg*>& virtualRegs)
        {
            const abi::CallingConvention* callingConvention = function->getCallingConvention();

            std::vector<Argument*> todo;

            for (auto it = function->mArguments.begin(); it != function->mArguments.end(); ++it)
            {
                auto& arg = *it;
                if (arg->getType()->isStructType())
                {
                    auto structType = static_cast<StructType*>(arg->getType());
                    if (structType->getSizeInBits() <= callingConvention->getMaxStructSize())
                    {

                    }
                }
            }

            int argumentIndex = 0;
            
            std::vector<Argument*> memArgs;
            
            auto iterations = std::min(callingConvention->getParameterRegisterCount(), static_cast<int>(function->mArguments.size()));
            for (int i = 0; i < iterations; ++i)
            {
                if (function->mArguments[i]->getType()->isStructType())
                {
                    auto structType = static_cast<StructType*>(function->mArguments[i]->getType());
                    if (structType->getSizeInBits() <= callingConvention->getMaxStructSize())
                    {
                        memArgs.push_back(function->mArguments[i].get());
                        continue;
                    }
                }
                
                auto it = std::find_if(virtualRegs.begin(), virtualRegs.end(), [argumentIndex, function](const auto& vreg) {
                    return vreg.second->mPhysicalRegister == function->getCallingConvention()->getParameterRegister(argumentIndex);
                });
                ++argumentIndex;
                auto& disallowed = function->mArguments[i]->mDisallowedVRegs;
                if (std::find(disallowed.begin(), disallowed.end(), it->second) != disallowed.end())
                {
                    todo.push_back(function->mArguments[i].get());
                }

                function->mArguments[i]->mVReg = it->second;
                virtualRegs.erase(it);
                activeValues.push_back(function->mArguments[i].get());
            }
            
            for (auto it = function->mArguments.begin() + callingConvention->getParameterRegisterCount(); it < function->mArguments.end(); ++it)
            {
                memArgs.push_back(it->get());
            }
            
            if (!memArgs.empty())
            {
                if (callingConvention->getArgumentPassingOrder() == abi::ArgumentPassingOrder::RightToLeft)
                {
                    addStackParams(memArgs.rbegin(), memArgs.rend(), function, abi, activeValues);
                }
                else if (callingConvention->getArgumentPassingOrder() == abi::ArgumentPassingOrder::LeftToRight)
                {
                    addStackParams(memArgs.begin(), memArgs.end(), function, abi, activeValues);
                }
            }

            return todo;
        }
    }
}