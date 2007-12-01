include_directories (${gdsto3d_SOURCE_DIR}/libgdsto3d ${OPENGL_INCLUDES})
link_directories (${gdsto3d_BINARY_DIR}/libgdsto3d)

add_executable (gdsoglviewer WIN32 gdsoglviewer.cpp gdsobject_ogl.cpp gdsparse_ogl.cpp)
target_link_libraries (gdsoglviewer gdsto3d ${OPENGL_LIBRARIES})

install (TARGETS gdsoglviewer RUNTIME DESTINATION ${BINDEST})
