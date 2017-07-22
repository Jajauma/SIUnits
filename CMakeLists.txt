cmake_minimum_required(VERSION 3.0.0)
project(SIUnits CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
add_executable(UnitsTests
    FrequencyTests.cpp
    UnitsTests.cpp)
target_link_libraries(UnitsTests GTest::Main)
