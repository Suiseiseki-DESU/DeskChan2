find_package(Qt5Widgets)

if(Qt5Widgets_FOUND)
	find_package(Qt5QuickWidgets)

	set(SRC_LIST Plugin.cpp Plugin.h CharacterWidget.cpp CharacterWidget.h SettingsDialog.cpp SettingsDialog.h
			QMLPluginProxy.cpp QMLPluginProxy.h)
	add_library(gui SHARED ${SRC_LIST})
	target_link_libraries(gui Qt5::Widgets ${PROJECT_NAME}_System)

	if (Qt5QuickWidgets_FOUND)
		target_link_libraries(gui Qt5::QuickWidgets)
	endif()

	install(TARGETS gui DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins/gui)
	install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins/gui)

	add_custom_command(
			TARGET gui POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
			COMMENT "Copying assets for gui plugin"
	)
endif()
