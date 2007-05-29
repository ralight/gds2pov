include_directories(${gdsto3d_SOURCE_DIR}/libgdsto3d)
link_directories(${gdsto3d_BINARY_DIR}/libgdsto3d)

add_executable (gds2info gds2info.cpp gdsobject_info.cpp gdsparse_info.cpp)
target_link_libraries (gds2info gdsto3d)
