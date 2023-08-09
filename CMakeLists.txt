cmake_minimum_required (VERSION 3.26)

project (vipir)

include_directories(include)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB vipir_sources
    src/vipir/Module.cpp

    src/vipir/IR/Function.cpp
)

add_library(vipir SHARED ${vipir_sources})

target_compile_features(vipir PUBLIC cxx_std_20)