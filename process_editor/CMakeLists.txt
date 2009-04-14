include(${wxWidgets_USE_FILE})
#include_directories (${gdsto3d_SOURCE_DIR}/libgdsto3d ${wxWidgets_INCLUDE_DIRS})
include_directories (${gdsto3d_SOURCE_DIR}/libgdsto3d)
link_directories (${gdsto3d_BINARY_DIR}/libgdsto3d)

add_executable (process_editor gpe_app.cpp gpe_window_fb.cpp gpe_window.cpp)
target_link_libraries (process_editor gdsto3d ${wxWidgets_LIBRARIES})

install(TARGETS process_editor RUNTIME DESTINATION ${BINDEST})

