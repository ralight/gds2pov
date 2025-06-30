add_executable(gds2openscad
	gds2openscad.cpp
	gdsobject_openscad.cpp gdsobject_openscad.h
	gdsparse_openscad.cpp gdsparse_openscad.h
)

target_link_libraries(gds2openscad PRIVATE
	gdsto3d
)

install(TARGETS gds2openscad RUNTIME DESTINATION ${BINDEST})
