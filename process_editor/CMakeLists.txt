include(${wxWidgets_USE_FILE})

add_executable(process_editor
	gpe_app.cpp
	gpe_window_fb.cpp
	gpe_window.cpp
	gpe_about.cpp
)
target_link_libraries(process_editor PRIVATE
	gdsto3d
	${wxWidgets_LIBRARIES}
)

install(TARGETS process_editor RUNTIME DESTINATION ${BINDEST})
