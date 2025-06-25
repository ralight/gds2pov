set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(GDSTO3D_SRCS
	gds_globals.cpp gds_globals.h
	gdsobject.cpp gdsobject.h
	gdsparse.cpp gdsparse.h
	gdsparse_basic_types.cpp
	gdspath.cpp gdspath.h
	gdspolygon.cpp gdspolygon.h
	gdstext.cpp gdstext.h
	gdsto3d.h
	process_cfg.cpp process_cfg.h
)

add_library(gdsto3d SHARED
	${GDSTO3D_SRCS}
)
set_target_properties(gdsto3d PROPERTIES
	VERSION 1.0.0
	SOVERSION 1
)
target_include_directories(gdsto3d PUBLIC
	${gdsto3d_SOURCE_DIR}/libgdsto3d
)

if(WIN32)
	target_link_libraries(gdsto3d PRIVATE
		ws2_32
	)
endif()

install(TARGETS gdsto3d
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
if(UNIX)
	install(CODE "EXEC_PROGRAM(/sbin/ldconfig)")
endif()
