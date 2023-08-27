// Copyright 2023 solar-mist

/*
 * A module represents a single translation unit(a file)
 * to be compiled using vipir
*/

#ifndef VIPIR_MODULE_H
#define VIPIR_MODULE_H 1

#include "vipir/IR/Global.h"

#include <memory>
#include <string>
#include <vector>

namespace vipir
{
    enum class OptimizationLevel
    {
        None,
        Low,
        Medium,
        High,
    };

    enum class OutputFormat
    {
        ELF,
        PE,
    };

    class Module
    {
    public:
        Module(std::string name);

        std::string_view getName() const;
        const std::vector<GlobalPtr>& getGlobals() const;
        
        void insertGlobal(Global* global);
        instruction::OperandPtr getGlobalEmittedValue(ValueId id);

        void print(std::ostream& stream) const;

        void optimize(OptimizationLevel level);

        void emit(std::ostream& stream, OutputFormat outputFormat) const;
    private:
        std::string mName;
        std::vector<GlobalPtr> mGlobals;
    };

    Value* getPointerOperand(Value* value);
}

#endif // VIPIR_MODULE_H