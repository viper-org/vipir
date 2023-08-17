cmake_minimum_required (VERSION 3.26)

project (vipir)

include(FetchContent)
FetchContent_Declare(vasm
GIT_REPOSITORY https://github.com/viper-org/vasm.git
GIT_TAG master)
FetchContent_MakeAvailable(vasm)

file(GLOB vipir_sources
    src/vipir/Module.cpp


    src/vipir/IR/Builder.cpp
    src/vipir/IR/Function.cpp
    src/vipir/IR/BasicBlock.cpp

    src/vipir/IR/Constant/ConstantInt.cpp

    src/vipir/IR/Instruction/RetInst.cpp
    src/vipir/IR/Instruction/AllocaInst.cpp
    src/vipir/IR/Instruction/StoreInst.cpp
    src/vipir/IR/Instruction/LoadInst.cpp
    src/vipir/IR/Instruction/BinOpInst.cpp


    src/vipir/Type/Type.cpp
    src/vipir/Type/VoidType.cpp
    src/vipir/Type/IntegerType.cpp


    src/vipir/MC/CmpOperand.cpp
)

add_library(vipir SHARED ${vipir_sources})

add_dependencies(vipir vasm::framework)

target_compile_features(vipir PUBLIC cxx_std_20)
target_include_directories(vipir PUBLIC include)