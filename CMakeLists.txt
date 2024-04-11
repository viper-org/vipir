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
)

set(HEADERS
    "include/vipir/Module.h"
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