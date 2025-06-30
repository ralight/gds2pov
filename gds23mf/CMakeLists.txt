add_executable(gds23mf
	gds23mf.cpp
	gdsobject_3mf.cpp gdsobject_3mf.h
	gdsparse_3mf.cpp gdsparse_3mf.h
)

find_package(lib3mf CONFIG REQUIRED)
target_link_libraries(gds23mf PUBLIC
	gds2x
	lib3mf::lib3mf
)

install(TARGETS gds23mf RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
