message(STATUS "Loading tests target...")

# ------------------------------------------------------------------------------ #
#                              Load sources                                      #
# ------------------------------------------------------------------------------ #

file(GLOB TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# ------------------------------------------------------------------------------ #
#                               Define exec                                      #
# ------------------------------------------------------------------------------ #

add_executable(${TEST_NAME} ${TEST_SOURCES})

# ------------------------------------------------------------------------------ #
#                                   Link                                         #
# ------------------------------------------------------------------------------ #

target_link_libraries(${TEST_NAME} PRIVATE ${LIB_NAME} GTest::gtest_main)

target_include_directories(${TEST_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers)

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})