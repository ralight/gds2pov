INCLUDE_DIRECTORIES (${gdsto3d_SOURCE_DIR}/libgdsto3d)
LINK_DIRECTORIES (${gdsto3d_BINARY_DIR}/libgdsto3d)

ADD_EXECUTABLE (gds2svg gds2svg.cpp gdsobject_svg.cpp gdsparse_svg.cpp)
TARGET_LINK_LIBRARIES (gds2svg gdsto3d)

INSTALL (TARGETS gds2svg RUNTIME DESTINATION ${BINDEST})
