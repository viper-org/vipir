cmake_minimum_required (VERSION 3.26)

project (vipir)

include_directories(include)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB vipir_sources
    src/vipir/Module.cpp

    src/vipir/IR/Builder.cpp
    src/vipir/IR/Function.cpp
    src/vipir/IR/BasicBlock.cpp

    src/vipir/IR/Instruction/RetInst.cpp

    src/vipir/Type/Type.cpp
    src/vipir/Type/VoidType.cpp
)

add_library(vipir SHARED ${vipir_sources})

target_compile_features(vipir PUBLIC cxx_std_20)