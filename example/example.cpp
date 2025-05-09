#include "vipir/Module.h"

#include "vipir/Pass/DefaultPass.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/GlobalVar.h"
#include "vipir/IR/GlobalString.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/IRBuilder.h"
#include "vipir/IR/Constant/ConstantBool.h"
#include "vipir/IR/Constant/ConstantInt.h"
#include "vipir/IR/Constant/ConstantStruct.h"
#include "vipir/IR/Constant/ConstantArray.h"
#include "vipir/IR/Instruction/AllocaInst.h"
#include "vipir/IR/Instruction/AddrInst.h"
#include "vipir/IR/Instruction/CallInst.h"
#include "vipir/IR/Instruction/BinaryInst.h"
#include "vipir/IR/Instruction/LoadInst.h"
#include "vipir/IR/Instruction/GEPInst.h"
#include "vipir/IR/Instruction/TruncInst.h"
#include "vipir/IR/Instruction/SelectInst.h"
#include "vipir/IR/Instruction/SExtInst.h"
#include "vipir/IR/Instruction/ZExtInst.h"
#include "vipir/IR/Instruction/PhiInst.h"
#include "vipir/IR/Instruction/RetInst.h"

#include "vipir/Optimizer/Peephole/PeepholeV2.h"
#include "vipir/Optimizer/RegAlloc/RegAlloc.h"
#include "vipir/Optimizer/Mem2Reg/Mem2Reg.h"

#include "vipir/Type/FunctionType.h"
#include "vipir/Type/StructType.h"
#include "vipir/Type/ArrayType.h"
#include "vipir/Type/Type.h"

#include "vipir/ABI/SysV.h"

#include "vipir/DI/DIVariable.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

class ExampleCallingConvention : public vipir::abi::CallingConvention
{
public:
    int getReturnRegister() const override
    {
        return 8;
    }

    int getParameterRegister(int index) const override
    {
        constexpr std::array registers { 2, 1, 3 };

        if (index >= registers.size()) return -1;

        return registers[index];
    }

    int getParameterRegisterCount() const override
    {
        return 3;
    }

    int getMaxStructSize() const override
    {
        return 128; // 16 bytes
    }

    std::vector<int> getCallerSavedRegisters() const override
    {
        return { 0, 1, 2, 6, 7, 8, 9, 10, 11 };
    }

    std::vector<int> getCalleeSavedRegisters() const override
    {
        return { 3, 12, 13, 14, 15 };
    }

    vipir::abi::ArgumentPassingOrder getArgumentPassingOrder() const override
    {
        return vipir::abi::ArgumentPassingOrder::LeftToRight;
    }

    vipir::abi::StackCleaner getStackCleaner() const override
    {
        return vipir::abi::StackCleaner::Callee;
    }

    std::string decorateName(std::string_view name, vipir::FunctionType* type) const override
    {
        return std::format("test_{}", name);
    }
};

int main()
{
    vipir::Module mod("test.tst");
    mod.setABI<vipir::abi::SysV>();

    mod.getPassManager().insertBefore(vipir::PassType::LIREmission, std::make_unique<vipir::ConstantFoldingPass>());
    mod.getPassManager().insertBefore(vipir::PassType::LIREmission, std::make_unique<vipir::opt::DCEPass>());
    //mod.getPassManager().insertBefore(vipir::PassType::LIREmission, std::make_unique<vipir::opt::PeepholePass>());

    vipir::IRBuilder builder;

    auto i32Type = vipir::Type::GetIntegerType(32);
    auto i64Type = vipir::Type::GetIntegerType(64);
    auto i8Type = vipir::Type::GetIntegerType(8);
    auto voidType = vipir::Type::GetVoidType();
    auto boolType = vipir::Type::GetBooleanType();

    auto func1 = vipir::Function::Create(vipir::FunctionType::Create(voidType, { boolType, boolType }), mod, "select_test", false);
    auto entrybb = vipir::BasicBlock::Create("", func1);

    builder.setInsertPoint(entrybb);

    auto const0 = builder.CreateConstantBool(false);
    auto arg0 = func1->getArgument(0);
    auto arg1 = func1->getArgument(1);

    auto select = builder.CreateSelect(arg0, arg1, const0);

    auto truebb = vipir::BasicBlock::Create("", func1);
    auto falsebb = vipir::BasicBlock::Create("", func1);

    builder.CreateCondBr(builder.CreateCmpNE(select, const0), truebb, falsebb);

    builder.setInsertPoint(truebb);
    builder.CreateRet(nullptr);

    builder.setInsertPoint(falsebb);
    builder.CreateRet(nullptr);

    mod.setOutputFormat(vipir::OutputFormat::ELF);

    std::ofstream exampleCode("test");

    mod.emit(exampleCode);

    mod.printLIR(std::cout);

    //mod.emit(std::cout);
}