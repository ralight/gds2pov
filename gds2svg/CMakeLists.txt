add_executable(gds2svg
	gds2svg.cpp
	gdsobject_svg.cpp gdsobject_svg.h
	gdsparse_svg.cpp gdsparse_svg.h
)

target_link_libraries(gds2svg PRIVATE
	gds2x
)

install(TARGETS gds2svg RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
