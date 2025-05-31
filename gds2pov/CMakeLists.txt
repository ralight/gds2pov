add_executable(gds2pov
	gds2pov.cpp
)

target_link_libraries(gds2pov PRIVATE
	gdsto3d
)

install(TARGETS gds2pov RUNTIME DESTINATION ${BINDEST})
