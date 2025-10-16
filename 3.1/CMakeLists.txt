cmake_minimum_required(VERSION 3.14)
project(NthPowerProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
include(GoogleTest)
find_package(GTest REQUIRED)

add_executable(
  run_tests
  tests/run_tests.cpp
)

target_include_directories(run_tests PRIVATE src)

target_link_libraries(
  run_tests
  PRIVATE GTest::gtest GTest::gtest_main
)

gtest_discover_tests(run_tests)
