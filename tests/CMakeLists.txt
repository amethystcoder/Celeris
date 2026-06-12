cmake_minimum_required(VERSION 3.16)
#project(celeris_test)

include(GoogleTest)
include(FetchContent)

set(TEST_NAME celeris_test)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# Prevent overriding the parent project's compiler/linker settings in Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable( ${TEST_NAME}
  connection_test.cpp
)
target_link_libraries( ${TEST_NAME} PUBLIC
  gtest_main
)

gtest_discover_tests(celeris_test)

