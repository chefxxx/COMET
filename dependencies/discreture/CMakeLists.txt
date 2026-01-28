# ------------------------------------------------------------------------------ #
#                               Add discreture                                   #
# ------------------------------------------------------------------------------ #

if (BUILD_TESTS)
    message(STATUS "Adding discreture...")

    include(FetchContent)

    FetchContent_Declare(
            discreture
            GIT_REPOSITORY https://github.com/mraggi/discreture.git
            GIT_TAG main
            GIT_PROGRESS TRUE
    )

    add_library(discreture INTERFACE)
    FetchContent_MakeAvailable(discreture)
endif ()