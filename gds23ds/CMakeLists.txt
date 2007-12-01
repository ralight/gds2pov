include_directories (${gdsto3d_SOURCE_DIR}/libgdsto3d)
link_directories (${gdsto3d_BINARY_DIR}/libgdsto3d)

add_executable (gds23ds gds23ds.cpp gdsobject_3ds.cpp gdsparse_3ds.cpp)
target_link_libraries (gds23ds gdsto3d)

install (TARGETS gds23ds RUTIME DESTINATION ${BINDEST})
