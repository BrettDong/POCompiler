cmake_minimum_required(VERSION 3.20)
project(POCompiler)

# Dependencies
message(STATUS "Preparing dependencies")

include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.9
)

FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 9.1.0
        )

FetchContent_MakeAvailable(doctest fmt)

# Project configuration

set(SOURCE
        src/FileWrapper.cpp
        src/FileWrapper.hpp
        src/LineView.hpp
        src/MemMappedFile.cpp
        src/MemMappedFile.hpp
        )

set(TEST_SOURCE
        tests/LineViewTest.cpp
        tests/TestMain.cpp)

add_library(Common INTERFACE)
target_include_directories(Common INTERFACE src)
target_link_libraries(Common INTERFACE fmt::fmt)

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