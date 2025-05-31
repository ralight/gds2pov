cmake_minimum_required(VERSION 3.10)

project (gdsto3d)
set (VERSION 20080229)

add_definitions (-DVERSION=\"${VERSION}\" -DHAVE_CONFIG_H -ggdb -Wall)
include_directories (${gdsto3d_BINARY_DIR})

include (CheckIncludeFile)
include (CheckFunctionExists)

find_library (WINSOCK2_LIBRARY "ws2_32")
check_include_file ("stdint.h" HAVE_STDINT_H)
check_include_file ("stdarg.h" HAVE_STDARG_H)
check_include_file ("sys/time.h" HAVE_SYS_TIME_H)
check_include_file ("time.h" HAVE_TIME_H)

check_include_file ("GL/gl.h" HAVE_GL_GL_H)
check_include_file ("GL/glu.h" HAVE_GL_GLU_H)
check_include_file ("GL/glx.h" HAVE_GL_GLX_H)

check_include_file ("X11/keysym.h" HAVE_X11_KEYSYM_H)
check_include_file ("X11/Xlib.h" HAVE_X11_XLIB_H)

check_include_file ("winsock2.h" HAVE_WINSOCK2_H)
check_include_file ("windows.h" HAVE_WINDOWS_H)

check_function_exists ("gettimeofday" HAVE_GETTIMEOFDAY)
check_function_exists ("QueryPerformanceCounter" HAVE_QUERYPERFORMANCECOUNTER)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Install destinations
if (WIN32)
	set (BINDEST .)
	set (LIBDEST .)
	set (SHAREDEST .)
else (WIN32)
	set (BINDEST bin)
	set (LIBDEST lib${LIB_SUFFIX})
	set (SHAREDEST share/gdsto3d)
endif (WIN32)

add_subdirectory (libgdsto3d)
add_subdirectory (gds2pov)
add_subdirectory (gds2svg)

SET(wxWidgets_USE_LIBS base core gl)
FIND_PACKAGE(wxWidgets)
IF(wxWidgets_FOUND)
#	add_subdirectory (gds2gui)
	add_subdirectory (process_editor)
ENDIF(wxWidgets_FOUND)

install(FILES changes.txt example.cmd example.gds example_process.txt example_config.txt readme.txt todo.txt DESTINATION ${SHAREDEST})

iF(UNIX)
	add_custom_target(dist ${PROJECT_SOURCE_DIR}/dist.sh ${PROJECT_SOURCE_DIR} ${gdsto3d_BINARY_DIR} ${VERSION})
endif(UNIX)
