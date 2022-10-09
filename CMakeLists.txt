cmake_minimum_required(VERSION 3.20)
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.24")
    cmake_policy(SET CMP0135 OLD)
endif ()
project(POCompiler)

# Dependencies
message(STATUS "Preparing dependencies")

include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(doctest
    URL https://github.com/doctest/doctest/archive/refs/tags/v2.4.9.tar.gz
)

FetchContent_Declare(fmt
    URL https://github.com/fmtlib/fmt/archive/refs/tags/9.1.0.tar.gz
)

if (BENCHMARK)
    FetchContent_Declare(googlebenchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.7.0.tar.gz
    )
    set(BENCHMARK_ENABLE_TESTING OFF)
endif ()

FetchContent_MakeAvailable(doctest fmt)

if (BENCHMARK)
    FetchContent_MakeAvailable(googlebenchmark)
endif ()

# Project configuration

set(SOURCE
        src/FileWrapper.cpp
        src/FileWrapper.hpp
        src/LineView.hpp
        src/MemMapFileReader.cpp
        src/MemMapFileReader.hpp
        src/Parser.cpp
        src/Parser.hpp
        src/StringUtil.cpp
        src/StringUtil.hpp
        src/Writer.cpp
        src/Writer.hpp
        )

set(TEST_SOURCE
        tests/LineViewTest.cpp
        tests/TestMain.cpp
        tests/StringUtilTest.cpp)

add_library(Common INTERFACE)
target_include_directories(Common INTERFACE src)
target_link_libraries(Common INTERFACE fmt::fmt)
if (SANITIZERS)
    target_compile_options(Common INTERFACE -fsanitize=address,undefined)
    target_link_options(Common INTERFACE -fsanitize=address,undefined)
endif ()
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if (COMPILER_SUPPORTS_MARCH_NATIVE)
    target_compile_options(Common INTERFACE -march=native)
endif ()
target_compile_options(Common INTERFACE -Wall -Wextra -Wpedantic -fno-omit-frame-pointer)

add_library(POCompilerCommon STATIC ${SOURCE})
target_link_libraries(POCompilerCommon Common)
set_target_properties(POCompilerCommon PROPERTIES CXX_STANDARD 17)
set_target_properties(POCompilerCommon PROPERTIES CXX_STANDARD_REQUIRED ON)

add_executable(POCompiler src/main.cpp)
target_link_libraries(POCompiler POCompilerCommon)
set_target_properties(POCompiler PROPERTIES CXX_STANDARD 17)
set_target_properties(POCompiler PROPERTIES CXX_STANDARD_REQUIRED ON)

add_executable(POCompilerTests ${TEST_SOURCE})
target_link_libraries(POCompilerTests POCompilerCommon doctest)
set_target_properties(POCompilerTests PROPERTIES CXX_STANDARD 17)
set_target_properties(POCompilerTests PROPERTIES CXX_STANDARD_REQUIRED ON)
enable_testing()
add_test(NAME POCompilerTests COMMAND $<TARGET_FILE:POCompilerTests>)

if (BENCHMARK)
    add_executable(POCompilerBenchmark benchmark/Benchmark.cpp)
    target_link_libraries(POCompilerBenchmark POCompilerCommon benchmark::benchmark)
    set_target_properties(POCompilerBenchmark PROPERTIES CXX_STANDARD 17)
    set_target_properties(POCompilerBenchmark PROPERTIES CXX_STANDARD_REQUIRED ON)
    target_compile_definitions(POCompilerBenchmark PRIVATE REF_PO_FILE="${BENCHMARK_REF_PO_FILE}")
endif ()
