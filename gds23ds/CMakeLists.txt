INCLUDE_DIRECTORIES (${gdsto3d_SOURCE_DIR}/libgdsto3d)
LINK_DIRECTORIES (${gdsto3d_BINARY_DIR}/libgdsto3d)

ADD_EXECUTABLE (gds23ds gds23ds.cpp gdsobject_3ds.cpp gdsparse_3ds.cpp)
TARGET_LINK_LIBRARIES (gds23ds gdsto3d)

INSTALL (TARGETS gds23ds RUTIME DESTINATION ${BINDEST})
