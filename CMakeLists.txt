cmake_minimum_required(VERSION 3.0.0)
project(SIUnits CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
include_directories(${CMAKE_CURRENT_LIST_DIR})
add_executable(Tests
    Tests/Demo/Frequency.cpp
    Tests/Demo/Length.cpp
    Tests/SIUnits.cpp)
target_link_libraries(Tests GTest::Main)
