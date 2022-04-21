
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(two_largest LANGUAGES CXX)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME benchmark
    GITHUB_REPOSITORY google/benchmark
    VERSION 1.6.1
    OPTIONS
      "BENCHMARK_ENABLE_TESTING Off"
      # The REGEX feature test fails when gbench's cmake is run under CPM w/ gcc5.4 because it doesn't assume C++11
      # Additionally, attempting to set the CMAKE_CXX_VERSION here doesn't propogate to the feature test build
      # Therefore, we just disable the feature test and assume platforms we care about have a regex impl available
      "RUN_HAVE_STD_REGEX 0" #
  )


add_executable(original two_largest.cpp)
target_compile_features(original PRIVATE cxx_std_17)
target_compile_options(original PRIVATE -O3)
target_link_libraries(original PRIVATE benchmark benchmark_main pthread)