cmake_minimum_required(VERSION 3.14)
project(NthPowerProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(STATUS "Clang detected, forcing use of libc++.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  run_tests
  tests/run_tests.cc
)

target_include_directories(run_tests PRIVATE src)

target_link_libraries(
  run_tests
  PRIVATE GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)
