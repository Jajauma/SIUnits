cmake_minimum_required(VERSION 3.0.0)
project(Types CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GTest REQUIRED)
add_executable(SIUnits.Tests SIUnits.Tests.cpp)
target_link_libraries(SIUnits.Tests GTest::Main)
