find_package(Qt5Widgets)

if(Qt5Widgets_FOUND)

	set(SRC_LIST Plugin.cpp Plugin.h CharacterWidget.cpp CharacterWidget.h SettingsDialog.cpp SettingsDialog.h)
	add_library(gui SHARED ${SRC_LIST})
	target_link_libraries(gui Qt5::Widgets DeskChan_System)

	add_custom_command(
			TARGET gui POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
			COMMENT "Copying assets for gui plugin"
	)

endif()
