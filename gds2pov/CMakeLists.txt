include_directories (${gdsto3d_SOURCE_DIR}/libgdsto3d)
link_directories (${gdsto3d_BINARY_DIR}/libgdsto3d)

add_executable (gds2pov gds2pov.cpp)
target_link_libraries (gds2pov gdsto3d)

install(TARGETS gds2pov RUNTIME DESTINATION ${BINDEST})

