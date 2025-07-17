# ------------------------------------------------------------------------------ #
#                                Project setup                                   #
# ------------------------------------------------------------------------------ #

set(PROJECT_NAME "EventMixing2")
set(LIB_NAME ${PROJECT_NAME}_lib)
set(TEST_NAME ${PROJECT_NAME}_test)

cmake_minimum_required(VERSION 3.30)
project(${PROJECT_NAME} C CXX)

set(PROJECT_VERSION "1.0")
set(PROJECT_AUTHOR "Mateusz Mikiciuk & Mykhailo Shamrai")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------ #
#                                  Load dirs                                     #
# ------------------------------------------------------------------------------ #

add_subdirectory(lib)
add_subdirectory(dependencies)

# ------------------------------------------------------------------------------ #
#                               Compile options                                  #
# ------------------------------------------------------------------------------ #

string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UPPER)
message("Build type: " "${CMAKE_BUILD_TYPE}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message("Using AppleClang...")
    if(BUILD_TYPE_UPPER STREQUAL "DEBUG")
        target_compile_options(${LIB_NAME} INTERFACE
                -g
                -fsanitize=undefined
                -fsanitize=address
                -O0
                -Wall
                -Wextra
                -pedantic
        )
        target_link_options(${LIB_NAME} INTERFACE
                -fsanitize=undefined
                -fsanitize=address
        )
    elseif (BUILD_TYPE_UPPER STREQUAL "RELEASE")
        target_compile_options(${LIB_NAME} INTERFACE -O3;-march=native;-funroll-loops;)
    else()
        message(FATAL_ERROR "Unknown build type: ${BUILD_TYPE_UPPER}")
    endif ()

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("Using GNU..")
    if(BUILD_TYPE_UPPER STREQUAL "DEBUG")
        target_compile_options(${LIB_NAME} INTERFACE
                -g
                -fsanitize=undefined
                -fsanitize=address
                -O0
                -Wall
                -Wextra
                -pedantic
        )
        target_link_options(${LIB_NAME} INTERFACE
                -fsanitize=undefined
                -fsanitize=address
        )
    elseif (BUILD_TYPE_UPPER STREQUAL "RELEASE")
        target_compile_options(${LIB_NAME} INTERFACE -O3;-march=native;-funroll-loops)
        message(FATAL_ERROR "UNKNOWN BUILD TYPE: ${BUILD_TYPE_UPPER}")
    endif()

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel" OR CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
    message(FATAL_ERROR "Intel compiler is not supported...")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(FATAL_ERROR "MSVC is not supported...")
else()
    message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

# ------------------------------------------------------------------------------ #
#                                   Load tests                                   #
# ------------------------------------------------------------------------------ #

enable_testing()
add_subdirectory(tests)