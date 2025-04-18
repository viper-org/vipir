#include "vipir/Module.h"

#include "vipir/Pass/DefaultPass.h"

#include "vipir/IR/Function.h"
#include "vipir/IR/GlobalVar.h"
#include "vipir/IR/GlobalString.h"
#include "vipir/IR/BasicBlock.h"
#include "vipir/IR/IRBuilder.h"
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

#include <dwarf.h>
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
    
    vipir::DIBuilder diBuilder(mod);
    diBuilder.setFilename("test.tst");
    diBuilder.setDirectory(std::filesystem::current_path().string());
    diBuilder.setProducer("Example vipIR project");
    mod.getPassManager().addPass(std::make_unique<vipir::DebugInfoEmissionPass>(&diBuilder));

    ExampleCallingConvention callingConvention;

    vipir::IRBuilder builder;

    auto i32Type = vipir::Type::GetIntegerType(32);
    auto i64Type = vipir::Type::GetIntegerType(64);
    auto i8Type = vipir::Type::GetIntegerType(8);
    auto voidType = vipir::Type::GetVoidType();
    auto boolType = vipir::Type::GetBooleanType();

    auto func1 = vipir::Function::Create(vipir::FunctionType::Create(i32Type, { i32Type }), mod, "main", false);
    //auto func2 = vipir::Function::Create(vipir::FunctionType::Create(voidType, { i32Type, i32Type, i32Type, i32Type, i32Type, i32Type, i32Type, i32Type }), mod, "test", false, &callingConvention);
    auto entrybb = vipir::BasicBlock::Create("", func1);
    //auto entrybb2 = vipir::BasicBlock::Create("", func2);

    builder.setInsertPoint(entrybb);

    builder.CreateDebugInfo(2, 5);
    auto alloca = builder.CreateAlloca(i32Type);
    auto val = vipir::ConstantInt::Get(mod, 5, i32Type);
    builder.CreateStore(alloca, val);
    //auto addr = builder.CreateAddrOf(alloca);

    //auto call = builder.CreateCall(func2, { vipir::ConstantInt::Get(mod, 1, i32Type), vipir::ConstantInt::Get(mod, 2, i32Type), vipir::ConstantInt::Get(mod, 3, i32Type), vipir::ConstantInt::Get(mod, 4, i32Type), vipir::ConstantInt::Get(mod, 5, i32Type), vipir::ConstantInt::Get(mod, 6, i32Type), vipir::ConstantInt::Get(mod, 7, i32Type), vipir::ConstantInt::Get(mod, 8, i32Type) });

    builder.CreateDebugInfo(5, 5);
    auto ret = builder.CreateRet(builder.CreateLoad(alloca));

    //builder.setInsertPoint(entrybb2);
    //auto ret2 = builder.CreateRet(nullptr);
    
    auto intDbgType = diBuilder.createDebugType("int", vipir::Type::GetIntegerType(32), DW_ATE_signed);
    //auto voidDbgType = diBuilder.createDebugType("void", vipir::Type::GetVoidType(), DW_ATE_void);

    diBuilder.setDebugType(func1, intDbgType);

    diBuilder.setSourceInfo(func1, 1, 3, 6, 1);

    diBuilder.createDebugVariable("x", func1, alloca, intDbgType, 4, 8);

    mod.setOutputFormat(vipir::OutputFormat::ELF);

    std::ofstream exampleCode("test");

    mod.emit(exampleCode);

    mod.printLIR(std::cout);

    //mod.emit(std::cout);
}