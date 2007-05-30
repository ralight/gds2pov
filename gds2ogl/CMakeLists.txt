include_directories(${gdsto3d_SOURCE_DIR}/libgdsto3d ${OPENGL_INCLUDES})
link_directories(${gdsto3d_BINARY_DIR}/libgdsto3d)

add_executable (gds2ogl gds2ogl.cpp gdsobject_ogl.cpp gdsparse_ogl.cpp)
target_link_libraries (gds2ogl gdsto3d ${OPENGL_LIBRARIES})
