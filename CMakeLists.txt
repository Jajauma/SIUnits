cmake_minimum_required(VERSION 3.0.0)
project(SIUnits CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
add_executable(Tests
    FrequencyTests.cpp
    LengthTests.cpp
    UnitsTests.cpp)
target_link_libraries(Tests GTest::Main)
