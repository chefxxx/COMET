message(STATUS "Loading benchmarks targets...")

file(GLOB BENCHMARK_SOURCES "*benchmark.cpp")

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

find_package(Boost)

if(Boost_FOUND)
    target_include_directories(discreture_vs_comet_full_combinations PRIVATE ${Boost_INCLUDE_DIRS})
    target_include_directories(discreture_vs_comet_strictly_upper PRIVATE ${Boost_INCLUDE_DIRS})
else()
    message("Boost not found! (Boost is required only fore discreture benchmarks!) Install it via 'brew install boost'")
endif()