INCLUDE_DIRECTORIES (${gdsto3d_SOURCE_DIR}/libgdsto3d ${OPENGL_INCLUDES})
LINK_DIRECTORIES (${gdsto3d_BINARY_DIR}/libgdsto3d)

ADD_EXECUTABLE (gdsoglviewer WIN32 gdsoglviewer.cpp gdsobject_ogl.cpp gdsparse_ogl.cpp)
TARGET_LINK_LIBRARIES (gdsoglviewer gdsto3d ${OPENGL_LIBRARIES})
