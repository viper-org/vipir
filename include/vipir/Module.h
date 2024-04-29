// Copyright 2024 solar-mist

/*
 * A module represents a single translation unit(a file)
 * to be compiled using vipir
*/

#ifndef VIPIR_MODULE_H
#define VIPIR_MODULE_H 1

#include "vipir/IR/Global.h"

#include "vipir/ABI/ABI.h"

#include <cassert>
#include <memory>
#include <string>
#include <vector>

namespace vipir
{
    enum class OutputFormat
    {
        ELF,
        PE,
    };

    class Module
    {
    using GlobalPtr = std::unique_ptr<Global>;

    public:
        Module(std::string name);

        template <class T>
        void setABI()
        {
            assert(mAbi == nullptr);
            mAbi = std::make_unique<T>();
        }

        abi::ABI* abi() const;
        std::string_view getName() const;
        int getNextValueId();

        void insertGlobal(Global* global);
        void insertGlobalAt(Global* global, int offset);

        void print(std::ostream& stream) const;

        void emit(std::ostream& stream, OutputFormat outputFormat) const;

    private:
        std::string mName;
        std::unique_ptr<abi::ABI> mAbi;
        std::vector<GlobalPtr> mGlobals;
    };

    Value* getPointerOperand(Value* value);
}

#endif // VIPIR_MODULE_H