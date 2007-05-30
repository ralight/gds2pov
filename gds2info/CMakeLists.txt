INCLUDE_DIRECTORIES (${gdsto3d_SOURCE_DIR}/libgdsto3d)
LINK_DIRECTORIES (${gdsto3d_BINARY_DIR}/libgdsto3d)

ADD_EXECUTABLE (gds2info gds2info.cpp gdsobject_info.cpp gdsparse_info.cpp)
TARGET_LINK_LIBRARIES (gds2info gdsto3d)

INSTALL (TARGETS gds2info RUNTIME DESTINATION ${BINDEST})
