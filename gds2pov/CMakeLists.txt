add_executable(gds2pov
	config_cfg.cpp config_cfg.h
	gds2pov.cpp
)

target_link_libraries(gds2pov PRIVATE
	gdsto3d
)

install(TARGETS gds2pov RUNTIME DESTINATION ${BINDEST})
