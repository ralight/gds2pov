include(${wxWidgets_USE_FILE})

add_executable(gds2gui
	gds2gui_app.cpp
	gds2gui_frame.cpp
)
target_link_libraries(gds2gui PRIVATE
	gdsto3d
	${wxWidgets_LIBRARIES}
)

install(TARGETS gds2gui RUNTIME DESTINATION ${BINDEST})
