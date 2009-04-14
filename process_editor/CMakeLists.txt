include_directories (${gdsto3d_SOURCE_DIR}/libgdsto3d)
link_directories (${gdsto3d_BINARY_DIR}/libgdsto3d)

add_executable (process_editor process_editor.cpp)
target_link_libraries (process_editor gdsto3d)

install(TARGETS process_editor RUNTIME DESTINATION ${BINDEST})

