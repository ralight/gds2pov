set (GDSTO3D_SRCS config_cfg.cpp gds_globals.cpp gdsobject.cpp gdsparse.cpp
		gdspath.cpp gdspolygon.cpp gdstext.cpp process_cfg.cpp
		gdsparse_pov.cpp gdsobject_pov.cpp
		gdsparse_svg.cpp gdsobject_svg.cpp)

add_library (gdsto3d SHARED ${GDSTO3D_SRCS})
set_target_properties (gdsto3d PROPERTIES VERSION 1.0.0 SOVERSION 1)

include_directories(${gdsto3d_SOURCE_DIR}/libgdsto3d)
link_directories(${gdsto3d_BINARY_DIR}/libgdsto3d)

if (WINSOCK2_LIBRARY)
	#target_link_libraries (gdsto3d ${WINSOCK2_LIBRARY})
	target_link_libraries (gdsto3d c:/unix/mingw/lib/libws2_32.a)
endif (WINSOCK2_LIBRARY)

install (TARGETS gdsto3d RUNTIME DESTINATION ${BINDEST} LIBRARY DESTINATION ${LIBDEST})
if (UNIX)
	install(CODE "EXEC_PROGRAM(/sbin/ldconfig)")
endif (UNIX)
