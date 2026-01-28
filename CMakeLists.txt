# ------------------------------------------------------------------------------ #
#                                Project setup                                   #
# ------------------------------------------------------------------------------ #

set(PROJECT_NAME "COMET")
set(LIB_NAME ${PROJECT_NAME}_lib)
set(TEST_NAME ${PROJECT_NAME}_test)

cmake_minimum_required(VERSION 3.30)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(${PROJECT_NAME} C CXX)

set(PROJECT_VERSION "1.0")
set(PROJECT_AUTHOR "Mateusz Mikiciuk & Mykhailo Shamrai")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------ #
#                                  Load dirs                                     #
# ------------------------------------------------------------------------------ #

add_subdirectory(lib/include)
add_subdirectory(dependencies)

# ------------------------------------------------------------------------------ #
#                               Compile options                                  #
# ------------------------------------------------------------------------------ #

# Set build type to Release by default:
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type provided, default to Release.")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

# Set cmake build type variable
string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UPPER)

message("Build type: " "${CMAKE_BUILD_TYPE}")
message("Using host compiler " ${CMAKE_CXX_COMPILER_ID})

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(${LIB_NAME} INTERFACE
            $<$<CONFIG:Debug>: -g -fsanitize=undefined  -fsanitize=address -O0 -Wall -Wextra -pedantic -Werror>
            $<$<CONFIG:Release>: -O2 -march=native -funroll-loops>
    )
    target_link_options(${LIB_NAME} INTERFACE
            $<$<CONFIG:Debug>: -fsanitize=undefined -fsanitize=address>
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${LIB_NAME} INTERFACE
            $<$<CONFIG:Debug>: -g -fsanitize=undefined  -fsanitize=address -O0 -Wall -Wextra -pedantic -Werror>
            $<$<CONFIG:Release>: -O2 -march=native -funroll-loops>
    )
    target_link_options(${LIB_NAME} INTERFACE
            $<$<CONFIG:Debug>: -fsanitize=undefined -fsanitize=address>
    )
else ()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER_ID} not supported!")
endif ()

# ------------------------------------------------------------------------------ #
#                                   Load tests                                   #
# ------------------------------------------------------------------------------ #

option(COMET_BUILD_TESTS "Build tests, benchmarks, and examples" OFF)

if (COMET_BUILD_TESTS)
    enable_testing()
    add_subdirectory(examples)
    add_subdirectory(tests)
    add_subdirectory(benchmarks)
endif()