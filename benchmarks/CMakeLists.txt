message(STATUS "Loading benchmarks targets...")

file(GLOB BENCHMARK_SOURCES "*.cpp")

foreach(bench_source ${BENCHMARK_SOURCES})

    get_filename_component(bench_name ${bench_source} NAME_WE)

    set(TARGET_NAME ${bench_name})

    message(STATUS "Registering benchmark target: ${TARGET_NAME}")

    add_executable(${TARGET_NAME} ${bench_source})

    target_link_libraries(${TARGET_NAME} PRIVATE
            ${LIB_NAME}
            benchmark::benchmark
            benchmark::benchmark_main
    )

    target_include_directories(${TARGET_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}/benchmarks/helpers
            ${CMAKE_SOURCE_DIR}/lib
    )

endforeach()

# 1. Find the Boost package (only headers are needed for static_vector)
find_package(Boost REQUIRED)

# 2. If Boost is found, add the include directories to your target
if(Boost_FOUND)
    target_include_directories(discreture_vs_comet_full_combinations PRIVATE ${Boost_INCLUDE_DIRS})
    target_include_directories(discreture_vs_comet_strictly_upper PRIVATE ${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found! Install it via 'brew install boost'")
endif()