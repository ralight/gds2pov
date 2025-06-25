cmake_minimum_required(VERSION 3.11)

project(gdsto3d
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VERSION 20080229)

add_definitions(-DVERSION=\"${VERSION}\")
if(WIN32)
	add_definitions("-DNOMINMAX")
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
	add_definitions("-D_CRT_NONSTDC_NO_DEPRECATE")
endif()

add_subdirectory(libgdsto3d)
add_subdirectory(gds2pov)
add_subdirectory(gds2svg)

SET(wxWidgets_USE_LIBS base core)
FIND_PACKAGE(wxWidgets)
IF(wxWidgets_FOUND)
	add_subdirectory(process_editor)
ENDIF(wxWidgets_FOUND)

install(FILES changes.txt example.cmd example.gds example_process.txt example_config.txt readme.txt todo.txt DESTINATION share/gdsto3d)

iF(UNIX)
	add_custom_target(dist ${PROJECT_SOURCE_DIR}/dist.sh ${PROJECT_SOURCE_DIR} ${gdsto3d_BINARY_DIR} ${VERSION})
endif(UNIX)
