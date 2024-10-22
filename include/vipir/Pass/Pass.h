// Copyright 2024 solar-mist

/*
 * A pass represents a single phase in the
 * translation process
*/

#ifndef VIPIR_PASS_PASS_H
#define VIPIR_PASS_PASS_H 1

#include <memory>
#include <vector>

namespace vipir
{
    class Module;

    enum class PassType
    {
        LIREmission,
        LIRCodegen,

        DeadCodeElimination,
        PeepholeOptimization,
        ConstantFolding,
    };

    class Pass
    {
    public:
        Pass(PassType type);

        virtual void execute(Module& module) = 0;

        PassType getType() const;

    private:
        PassType mType;
    };

    class PassManager
    {
    public:
        void addPass(std::unique_ptr<Pass> pass);
        int findPass(PassType type);
        void insertPass(int position, std::unique_ptr<Pass> pass);
        void insertBefore(PassType other, std::unique_ptr<Pass> pass);
        void insertAfter(PassType other, std::unique_ptr<Pass> pass);
        void removePass(PassType type);

        void runPasses(Module& module);

    private:
        std::vector<std::unique_ptr<Pass> > mPasses;
    };
}

#endif // VIPIR_PASS_PASS_H