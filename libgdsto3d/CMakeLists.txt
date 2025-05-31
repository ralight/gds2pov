set(GDSTO3D_SRCS
	config_cfg.cpp config_cfg.h
	gds_globals.cpp gds_globals.h
	gdsobject.cpp gdsobject.h
	gdsobject_pov.cpp gdsobject_pov.h
	gdsobject_svg.cpp gdsobject_svg.h
	gdsparse.cpp gdsparse.h
	gdsparse_pov.cpp gdsparse_pov.h
	gdsparse_svg.cpp gdsparse_svg.h
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
target_include_directories(gdsto3d INTERFACE
	${gdsto3d_SOURCE_DIR}/libgdsto3d
)

if(WIN32)
	target_link_libraries(gdsto3d PRIVATE
		ws2_32
	)
endif()

install(TARGETS gdsto3d RUNTIME DESTINATION ${BINDEST} LIBRARY DESTINATION ${LIBDEST})
if(UNIX)
	install(CODE "EXEC_PROGRAM(/sbin/ldconfig)")
endif()
