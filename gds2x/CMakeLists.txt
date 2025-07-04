add_executable(gds2x
	gds2x.cpp

	3mf/gdsobject_3mf.cpp
	3mf/gdsobject_3mf.h
	3mf/gdsparse_3mf.cpp
	3mf/gdsparse_3mf.h

	openscad/gdsobject_openscad.cpp
	openscad/gdsobject_openscad.h
	openscad/gdsparse_openscad.cpp
	openscad/gdsparse_openscad.h

	povray/config_cfg.cpp
	povray/config_cfg.h
	povray/gdsobject_pov.cpp
	povray/gdsobject_pov.h
	povray/gdsparse_pov.cpp
	povray/gdsparse_pov.h

	svg/gdsobject_svg.cpp
	svg/gdsobject_svg.h
	svg/gdsparse_svg.cpp
	svg/gdsparse_svg.h
)

find_package(lib3mf CONFIG REQUIRED)
target_include_directories(gds2x PUBLIC
	${gds2x_SOURCE_DIR}/gds2x/3mf
	${gds2x_SOURCE_DIR}/gds2x/openscad
	${gds2x_SOURCE_DIR}/gds2x/povray
	${gds2x_SOURCE_DIR}/gds2x/svg
)
target_link_libraries(gds2x PUBLIC
	libgds2x
	lib3mf::lib3mf
)

install(TARGETS gds2x RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
