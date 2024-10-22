// Copyright 2024 solar-mist

/*
 * A module represents a single translation unit(a file)
 * to be compiled using vipir
*/

#ifndef VIPIR_MODULE_H
#define VIPIR_MODULE_H 1

#include "vipir/IR/Global.h"
#include "vipir/IR/GlobalVar.h"

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
    using ValuePtr = std::unique_ptr<Value>;
    friend class Pass;
    public:
        Module(std::string name);

        template <class T>
        void setABI()
        {
            assert(mAbi == nullptr);
            mAbi = std::make_unique<T>();
        }

        PassManager& getPassManager();
        void runPasses();

        abi::ABI* abi() const;
        std::string_view getName() const;
        int getNextValueId();

        const std::vector<GlobalPtr>& getGlobals() const;
        GlobalVar* createGlobalVar(Type* type);
        void insertGlobal(Global* global);
        void insertGlobalAt(Global* global, int offset);
        void insertGlobalAtFront(Global* global);
        const std::vector<ValuePtr>& getConstants() const;
        void insertConstant(Value* constant);

        lir::Builder& getLIRBuilder();

        void print(std::ostream& stream) const;
        void printLIR(std::ostream& stream);

        void setOutputFormat(OutputFormat format);

        void lirEmission();
        void lirCodegen();
        void emit(std::ostream& stream);

    private:
        std::string mName;
        std::unique_ptr<abi::ABI> mAbi;
        std::vector<GlobalPtr> mGlobals;
        std::vector<ValuePtr> mConstants;
        
        PassManager mPassManager;
        bool mRunPasses;

        lir::Builder mBuilder;
        std::unique_ptr<codegen::IOutputFormat> mOutputFormat;
    };

    Value* getPointerOperand(Value* value);
}

#endif // VIPIR_MODULE_H