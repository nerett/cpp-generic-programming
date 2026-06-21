cmake_minimum_required(VERSION 3.24)
project(ExamSem2Project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(snapshot_set_test 2-SnapshotSet.cc)
target_link_libraries(snapshot_set_test PRIVATE GTest::gtest_main Threads::Threads)

add_executable(sieve_test 3-sieve.cc)
target_link_libraries(sieve_test PRIVATE GTest::gtest_main Threads::Threads)

include(GoogleTest)
gtest_discover_tests(snapshot_set_test)
gtest_discover_tests(sieve_test)
