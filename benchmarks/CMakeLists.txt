message(STATUS "Loading the benchmarks targets...")

file(GLOB BENCHMARK_SOURCES "*benchmark.cpp")

foreach(bench_source ${BENCHMARK_SOURCES})
    get_filename_component(bench_name ${bench_source} NAME_WE)
    set(TARGET_NAME ${bench_name})

    if (${TARGET_NAME} STREQUAL "stdVsCometFullCombinations_benchmark" AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
        message("The std::ranges::views::cartesian_product is not supported on the AppleClang!")
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

find_package(Boost)

if(Boost_FOUND)
    target_include_directories(discretureVsCometFullCombinations_benchmark PRIVATE ${Boost_INCLUDE_DIRS})
    target_include_directories(discretureVsCometStrictlyUpper_benchmark PRIVATE ${Boost_INCLUDE_DIRS})
else()
    message("The Boost not found, it is required to run the discreture benchmarks!")
endif()