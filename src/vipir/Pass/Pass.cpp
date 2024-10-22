// Copyright 2024 solar-mist

#include "vipir/Pass/Pass.h"

#include <algorithm>

namespace vipir
{
    Pass::Pass(PassType type)
        : mType(type)
    {
    }

    PassType Pass::getType() const
    {
        return mType;
    }


    void PassManager::addPass(std::unique_ptr<Pass> pass)
    {
        mPasses.push_back(std::move(pass));
    }

    int PassManager::findPass(PassType type)
    {
        auto it = std::find_if(mPasses.begin(), mPasses.end(), [type](const auto& pass){
            return pass->getType() == type;
        });
        if (it != mPasses.end()) return it - mPasses.begin();

        return -1;
    }

    void PassManager::insertPass(int position, std::unique_ptr<Pass> pass)
    {
        mPasses.insert(mPasses.begin() + position, std::move(pass));
    }

    void PassManager::insertBefore(PassType other, std::unique_ptr<Pass> pass)
    {
        auto position = findPass(other);
        if (position == -1) addPass(std::move(pass));
        else insertPass(position, std::move(pass));
    }

    void PassManager::insertAfter(PassType other, std::unique_ptr<Pass> pass)
    {
        auto position = findPass(other);
        if (position == -1) addPass(std::move(pass));
        else insertPass(position + 1, std::move(pass));
    }

    void PassManager::removePass(PassType type)
    {
        std::erase_if(mPasses, [type](const auto& pass){
            return pass->getType() == type;
        });
    }


    void PassManager::runPasses(Module& module)
    {
        for (auto& pass : mPasses)
        {
            pass->execute(module);
        }
    }
}