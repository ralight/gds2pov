INCLUDE_DIRECTORIES (${gdsto3d_SOURCE_DIR}/libgdsto3d)
LINK_DIRECTORIES (${gdsto3d_BINARY_DIR}/libgdsto3d)

ADD_EXECUTABLE (gds2pov gds2pov.cpp gdsobject_pov.cpp gdsparse_pov.cpp)
TARGET_LINK_LIBRARIES (gds2pov gdsto3d)
