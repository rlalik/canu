cmake_minimum_required(VERSION 3.14)

project(canuTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(canu REQUIRED)
  enable_testing()
endif()

include(FetchContent)

set(INSTALL_GTEST OFF)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723 # release-1.17.0
  FIND_PACKAGE_ARGS NAMES GTest
)

# This will try calling find_package() first for dependencies
FetchContent_MakeAvailable(googletest)

# ---- Tests ----

add_executable(canu_test
  source/tests_canvas_resize.cpp
  source/tests_canvas_margins.cpp
)
target_link_libraries(canu_test PRIVATE canu::canu GTest::gtest_main)
target_compile_features(canu_test PRIVATE cxx_std_17)

add_test(NAME canu_test COMMAND canu_test)

include(GoogleTest)
gtest_discover_tests(canu_test)

# ---- End-of-file commands ----

add_folders(Test)
