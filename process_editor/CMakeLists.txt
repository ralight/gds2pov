include(${wxWidgets_USE_FILE})

add_executable(gds2x_process_editor
	gpe_app.cpp
	gpe_window_fb.cpp
	gpe_window.cpp
	gpe_about.cpp
)
target_link_libraries(gds2x_process_editor PRIVATE
	gdsto3d
	${wxWidgets_LIBRARIES}
)

install(TARGETS gds2x_process_editor RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
