include(${wxWidgets_USE_FILE})

add_executable(gds2x_process_editor
	gpe_app.cpp
	gpe_window_fb.cpp
	gpe_window.cpp
	gpe_about.cpp
)
target_link_libraries(gds2x_process_editor PRIVATE
	gds2x
	${wxWidgets_LIBRARIES}
)

set_target_properties(gds2x_process_editor PROPERTIES
	WIN32_EXECUTABLE 1
)

install(TARGETS gds2x_process_editor RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
