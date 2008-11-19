include(${wxWidgets_USE_FILE})
include_directories (${gdsto3d_SOURCE_DIR}/libgdsto3d)
link_directories (${gdsto3d_BINARY_DIR}/libgdsto3d)

#add_executable (gds2gui gds2gui.cpp gds2gui_app.cpp gds2gui_frame.cpp)
add_executable (gds2gui gds2gui_app.cpp gds2gui_frame.cpp)
target_link_libraries (gds2gui gdsto3d ${wxWidgets_LIBRARIES})

install(TARGETS gds2gui RUNTIME DESTINATION ${BINDEST})

