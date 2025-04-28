// Copyright 2025 solar-mist

/*
 * Passes for emitting information
 * about the IR, such as CFG graphs
*/

#ifndef VIPIR_PASS_IR_INFO_PASS_H
#define VIPIR_PASS_IR_INFO_PASS_H 1

#include "vipir/Pass/Pass.h"

namespace vipir
{
    class CFGEmissionPass : public Pass
    {
    public:
        CFGEmissionPass(std::string outpath = "cfg.dot");

        virtual void execute(Module& module) override;

    private:
        std::string mOutPath;
    };
}

#endif // VIPIR_PASS_IR_INFO_PASS_H