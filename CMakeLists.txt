cmake_minimum_required(VERSION 3.14)
project(oop5 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_library(source 
    src/memory.cpp 
    src/memory.h 
    src/stack.h
)
target_include_directories(source PUBLIC ${CMAKE_SOURCE_DIR}/src)

# Исполняемый файл лабораторной
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE source)

enable_testing()

add_executable(tests tests/test.cpp)
target_link_libraries(tests PRIVATE source GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)