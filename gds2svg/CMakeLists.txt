add_executable(gds2svg
	gds2svg.cpp
)

target_link_libraries(gds2svg PRIVATE
	gdsto3d
)

install(TARGETS gds2svg RUNTIME DESTINATION ${BINDEST})
