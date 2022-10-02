cmake_minimum_required(VERSION 3.20)
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.24")
    cmake_policy(SET CMP0135 OLD)
endif ()
project(POCompiler)

# Dependencies
message(STATUS "Preparing dependencies")

include(FetchContent)

FetchContent_Declare(
        doctest
        URL https://github.com/doctest/doctest/archive/refs/tags/v2.4.9.tar.gz
)

FetchContent_Declare(fmt
        URL https://github.com/fmtlib/fmt/archive/refs/tags/9.1.0.tar.gz
        )

FetchContent_MakeAvailable(doctest fmt)

# Project configuration

set(SOURCE
        src/FileWrapper.cpp
        src/FileWrapper.hpp
        src/LineView.hpp
        src/MemMapFileReader.cpp
        src/MemMapFileReader.hpp
        )

set(TEST_SOURCE
        tests/LineViewTest.cpp
        tests/TestMain.cpp)

add_library(Common INTERFACE)
target_include_directories(Common INTERFACE src)
target_link_libraries(Common INTERFACE fmt::fmt)
if (SANITIZERS)
    target_compile_options(Common INTERFACE -fsanitize=address,undefined)
    target_link_options(Common INTERFACE -fsanitize=address,undefined)
endif ()

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