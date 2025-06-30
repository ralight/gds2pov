add_executable(gds2pov
	config_cfg.cpp config_cfg.h
	gds2pov.cpp
	gdsobject_pov.cpp gdsobject_pov.h
	gdsparse_pov.cpp gdsparse_pov.h
)

target_link_libraries(gds2pov PRIVATE
	gds2x
)

install(TARGETS gds2pov RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
