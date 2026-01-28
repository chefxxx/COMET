# ------------------------------------------------------------------------------ #
#                               Add discreture                                   #
# ------------------------------------------------------------------------------ #

if (COMET_BUILD_BENCHMARKS)
    message(STATUS "Adding discreture...")

    include(FetchContent)

    FetchContent_Declare(
            discreture
            GIT_REPOSITORY https://github.com/mraggi/discreture.git
            GIT_TAG master
            GIT_PROGRESS TRUE
    )

    set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(discreture)
endif ()