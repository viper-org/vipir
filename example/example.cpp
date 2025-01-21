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

#include "vipir/Optimizer/Peephole/PeepholeV2.h"
#include "vipir/Optimizer/RegAlloc/RegAlloc.h"
#include "vipir/Optimizer/Mem2Reg/Mem2Reg.h"

#include "vipir/Type/FunctionType.h"
#include "vipir/Type/StructType.h"
#include "vipir/Type/ArrayType.h"
#include "vipir/Type/Type.h"

#include "vipir/ABI/SysV.h"

#include <iostream>
#include <fstream>
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
        return std::format("freaky_{}", name);
    }
};

int main()
{
    vipir::Module mod("test.tst");
    mod.setABI<vipir::abi::SysV>();

    ExampleCallingConvention callingConvention;

    vipir::IRBuilder builder;

    auto i32Type = vipir::Type::GetIntegerType(32);
    auto i64Type = vipir::Type::GetIntegerType(64);
    auto i8Type = vipir::Type::GetIntegerType(8);
    auto voidType = vipir::Type::GetVoidType();
    auto boolType = vipir::Type::GetBooleanType();

    auto func1 = vipir::Function::Create(vipir::FunctionType::Create(i32Type, { i32Type }), mod, "main", false);
    auto func2 = vipir::Function::Create(vipir::FunctionType::Create(voidType, { i32Type, i32Type, i32Type, i32Type, i32Type, i32Type, i32Type, i32Type }), mod, "test", false, &callingConvention);
    auto entrybb = vipir::BasicBlock::Create("", func1);
    auto entrybb2 = vipir::BasicBlock::Create("", func2);

    builder.setInsertPoint(entrybb);

    auto alloca = builder.CreateAlloca(i32Type);
    auto val = vipir::ConstantInt::Get(mod, 5, i32Type);
    builder.CreateStore(alloca, val);
    auto addr = builder.CreateAddrOf(alloca);

    builder.CreateCall(func2, { vipir::ConstantInt::Get(mod, 1, i32Type), vipir::ConstantInt::Get(mod, 2, i32Type), vipir::ConstantInt::Get(mod, 3, i32Type), vipir::ConstantInt::Get(mod, 4, i32Type), vipir::ConstantInt::Get(mod, 5, i32Type), vipir::ConstantInt::Get(mod, 6, i32Type), vipir::ConstantInt::Get(mod, 7, i32Type), vipir::ConstantInt::Get(mod, 8, i32Type) });

    builder.CreateRet(builder.CreateLoad(alloca));

    builder.setInsertPoint(entrybb2);
    builder.CreateRet(nullptr);

    mod.setOutputFormat(vipir::OutputFormat::ELF);

    std::ofstream exampleCode("test");

    mod.emit(exampleCode);

    mod.printLIR(std::cout);

    //mod.emit(std::cout);
}
