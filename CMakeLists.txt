cmake_minimum_required(VERSION 3.11)

project(gdsto3d)
set(VERSION 20080229)

add_definitions(-DVERSION=\"${VERSION}\" -ggdb -Wall)
include_directories(${gdsto3d_BINARY_DIR})

# Install destinations
if(WIN32)
	set(BINDEST .)
	set(LIBDEST .)
	set(SHAREDEST .)
else(WIN32)
	set(BINDEST bin)
	set(LIBDEST lib${LIB_SUFFIX})
	set(SHAREDEST share/gdsto3d)
endif(WIN32)

add_subdirectory(libgdsto3d)
add_subdirectory(gds2pov)
add_subdirectory(gds2svg)

SET(wxWidgets_USE_LIBS base core)
FIND_PACKAGE(wxWidgets)
IF(wxWidgets_FOUND)
#	add_subdirectory(gds2gui)
	add_subdirectory(process_editor)
ENDIF(wxWidgets_FOUND)

install(FILES changes.txt example.cmd example.gds example_process.txt example_config.txt readme.txt todo.txt DESTINATION ${SHAREDEST})

iF(UNIX)
	add_custom_target(dist ${PROJECT_SOURCE_DIR}/dist.sh ${PROJECT_SOURCE_DIR} ${gdsto3d_BINARY_DIR} ${VERSION})
endif(UNIX)
