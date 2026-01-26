message(STATUS "Loading the benchmarks targets...")

find_package(Boost)
file(GLOB BENCHMARK_SOURCES "*benchmark.cpp")

foreach(bench_source ${BENCHMARK_SOURCES})
    get_filename_component(bench_name ${bench_source} NAME_WE)
    set(TARGET_NAME ${bench_name})

    if (${TARGET_NAME} STREQUAL "stdVsCometFullCombinations_benchmark" AND (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "13.0")))
        message("The std::ranges::views::cartesian_product is not supported on the AppleClang!")
        message("Skipping the benchmark build...")
        continue()
    endif ()

    if (${TARGET_NAME} STREQUAL "discretureVsCometFullCombinations_benchmark" OR ${TARGET_NAME} STREQUAL "discretureVsCometStrictlyUpper_benchmark" AND NOT Boost_FOUND)
        message("The Boost not found, it is required to run the discreture benchmarks!")
        message("Skipping the benchmark build...")
        continue()
    endif ()

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