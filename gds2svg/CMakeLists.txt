include_directories (${gdsto3d_SOURCE_DIR}/libgdsto3d)
link_directories (${gdsto3d_BINARY_DIR}/libgdsto3d)

add_executable (gds2svg gds2svg.cpp gdsobject_svg.cpp gdsparse_svg.cpp)
target_link_libraries (gds2svg gdsto3d)

install (TARGETS gds2svg RUNTIME DESTINATION ${BINDEST})
