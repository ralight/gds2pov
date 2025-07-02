set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # FIXME

find_package(nlohmann_json CONFIG REQUIRED)

set(GDS2X_SRCS
	gds_globals.cpp gds_globals.h
	gdsobject.cpp gdsobject.h
	gdsparse.cpp gdsparse.h
	gdsparse_basic_types.cpp
	gdspath.cpp gdspath.h
	gdspolygon.cpp gdspolygon.h
	gdstext.cpp gdstext.h
	gds2x.h
	process_cfg.cpp process_cfg.h

	PolygonTriangulator.cxx PolygonTriangulator.h
)

add_library(gds2x SHARED
	${GDS2X_SRCS}
)
set_target_properties(gds2x PROPERTIES
	VERSION 1.0.0
	SOVERSION 1
)
target_include_directories(gds2x PUBLIC
	${gds2x_SOURCE_DIR}/libgds2x
)

target_link_libraries(gds2x PRIVATE
	nlohmann_json::nlohmann_json
)

if(WIN32)
	target_link_libraries(gds2x PRIVATE
		ws2_32
	)
endif()

install(TARGETS gds2x
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
if(UNIX)
	install(CODE "EXEC_PROGRAM(/sbin/ldconfig)")
endif()
