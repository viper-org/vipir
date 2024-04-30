cmake_minimum_required (VERSION 3.26)

project (vipir)

include(FetchContent)
FetchContent_Declare(vasm
    GIT_REPOSITORY https://github.com/viper-org/vasm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(vasm)

set(SOURCES
    "src/vipir/Module.cpp"

    "src/vipir/IR/Function.cpp"
    "src/vipir/IR/Argument.cpp"
    "src/vipir/IR/GlobalString.cpp"
    "src/vipir/IR/GlobalVar.cpp"
    "src/vipir/IR/BasicBlock.cpp"
    "src/vipir/IR/IRBuilder.cpp"

    "src/vipir/IR/Constant/ConstantInt.cpp"
    "src/vipir/IR/Constant/ConstantBool.cpp"

    "src/vipir/IR/Instruction/Instruction.cpp"
    "src/vipir/IR/Instruction/RetInst.cpp"
    "src/vipir/IR/Instruction/AllocaInst.cpp"
    "src/vipir/IR/Instruction/StoreInst.cpp"
    "src/vipir/IR/Instruction/LoadInst.cpp"
    "src/vipir/IR/Instruction/AddrInst.cpp"
    "src/vipir/IR/Instruction/GEPInst.cpp"
    "src/vipir/IR/Instruction/BinaryInst.cpp"
    "src/vipir/IR/Instruction/UnaryInst.cpp"
    "src/vipir/IR/Instruction/BranchInst.cpp"
    "src/vipir/IR/Instruction/CallInst.cpp"

    "src/vipir/Type/Type.cpp"
    "src/vipir/Type/IntegerType.cpp"
    "src/vipir/Type/VoidType.cpp"
    "src/vipir/Type/BooleanType.cpp"
    "src/vipir/Type/FunctionType.cpp"
    "src/vipir/Type/PointerType.cpp"
    "src/vipir/Type/StructType.cpp"

    "src/vipir/MC/Builder.cpp"
    "src/vipir/MC/CmpOperand.cpp"

    "src/vipir/ABI/SysV.cpp"
)

set(HEADERS
    "include/vipir/Module.h"

    "include/vipir/IR/Value.h"
    "include/vipir/IR/Global.h"
    "include/vipir/IR/Function.h"
    "include/vipir/IR/Argument.h"
    "include/vipir/IR/GlobalString.h"
    "include/vipir/IR/GlobalVar.h"
    "include/vipir/IR/BasicBlock.h"
    "include/vipir/IR/IRBuilder.h"

    "include/vipir/IR/Constant/ConstantInt.h"
    "include/vipir/IR/Constant/ConstantBool.h"

    "include/vipir/IR/Instruction/Instruction.h"
    "include/vipir/IR/Instruction/RetInst.h"
    "include/vipir/IR/Instruction/AllocaInst.h"
    "include/vipir/IR/Instruction/StoreInst.h"
    "include/vipir/IR/Instruction/LoadInst.h"
    "include/vipir/IR/Instruction/AddrInst.h"
    "include/vipir/IR/Instruction/GEPInst.h"
    "include/vipir/IR/Instruction/BinaryInst.h"
    "include/vipir/IR/Instruction/UnaryInst.h"
    "include/vipir/IR/Instruction/BranchInst.h"
    "include/vipir/IR/Instruction/CallInst.h"

    "include/vipir/Type/Type.h"
    "include/vipir/Type/IntegerType.h"
    "include/vipir/Type/VoidType.h"
    "include/vipir/Type/BooleanType.h"
    "include/vipir/Type/FunctionType.h"
    "include/vipir/Type/PointerType.h"
    "include/vipir/Type/StructType.h"

    "include/vipir/MC/Builder.h"
    "include/vipir/MC/CmpOperand.h"

    "include/vipir/ABI/ABI.h"
    "include/vipir/ABI/SysV.h"
)

add_library(vipir ${SOURCES} ${HEADERS})
add_dependencies(vipir vasm::framework)
target_include_directories(vipir
    PUBLIC
        include
)
target_compile_features(vipir PUBLIC cxx_std_20)
target_include_directories(vipir PUBLIC include)
target_link_libraries(vipir PUBLIC vasm-framework-vasm-framework)

add_executable(vipir-example example/example.cpp)
target_compile_features(vipir-example PUBLIC cxx_std_20)
target_link_libraries(vipir-example PUBLIC vipir)