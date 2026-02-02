# ------------------------------------------------------------------------------ #
#                               Add google test                                  #
# ------------------------------------------------------------------------------ #

if (COMET_BUILD_TESTS)
    message(STATUS "Adding gtest...")

    include(FetchContent)

    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest
            GIT_TAG main
            GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(googletest)
endif ()
