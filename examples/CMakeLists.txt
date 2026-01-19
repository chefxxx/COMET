message("Loading examples...")

# ------------------------------------------------------------------------------ #
# Gather source files                                #
# ------------------------------------------------------------------------------ #

file(GLOB SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
)

# ------------------------------------------------------------------------------ #
# Create examples execs                              #
# ------------------------------------------------------------------------------ #

foreach(source_file ${SOURCES})
    get_filename_component(exe_name ${source_file} NAME_WE)
    add_executable(${exe_name} ${source_file})
    target_link_libraries(${exe_name} PRIVATE ${LIB_NAME})
endforeach()
