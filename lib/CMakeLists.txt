# ------------------------------------------------------------------------------ #
#                             Create header lib                                  #
# ------------------------------------------------------------------------------ #

add_library(${LIB_NAME} INTERFACE)
add_library(EM::${LIB_NAME} ALIAS ${LIB_NAME})
# ------------------------------------------------------------------------------ #
#                               Load files                                       #
# ------------------------------------------------------------------------------ #

target_include_directories(${LIB_NAME} INTERFACE $<BUILD_INTERFACE:${MAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include/EventMixing2>
	)

#install(TARGETS ${LIB_NAME}
#	EXPORT O2Targets
#	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
