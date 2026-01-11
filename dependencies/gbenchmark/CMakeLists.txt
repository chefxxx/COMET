# ------------------------------------------------------------------------------ #
#                               Add google benchmark                             #
# ------------------------------------------------------------------------------ #

message(STATUS "Adding gbenchmark...")

include(FetchContent)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)

FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(googlebenchmark)