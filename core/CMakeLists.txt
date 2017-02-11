add_definitions(-DDC_CORE_LIBRARY)

find_package(Qt5Widgets)

add_executable(${PROJECT_NAME} main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
target_link_libraries(${PROJECT_NAME} Qt5::Core)

set(SRC_LIST libmain.cpp DCPluginManager.cpp DCPluginManager.h DCPlugin.cpp DCPlugin.h DCCorePlugin.h DCCorePlugin.cpp
		DCCommon.h DCExternalPlugin.cpp DCExternalPlugin.h)
add_library(${PROJECT_NAME}_System SHARED ${SRC_LIST})
target_link_libraries(${PROJECT_NAME}_System Qt5::Core)

add_custom_command(
		TARGET ${PROJECT_NAME}_System POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}_System${CMAKE_SHARED_LIBRARY_SUFFIX}
		${CMAKE_CURRENT_BINARY_DIR}/..
)

if(Qt5Widgets_FOUND)
	add_library(${PROJECT_NAME}_GuiSupport SHARED libguisupport.cpp)
	target_link_libraries(${PROJECT_NAME}_GuiSupport Qt5::Widgets)
	add_custom_command(
			TARGET ${PROJECT_NAME}_GuiSupport POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}_GuiSupport${CMAKE_SHARED_LIBRARY_SUFFIX}
			${CMAKE_CURRENT_BINARY_DIR}/..
	)
	if(WIN32)
		get_target_property(QtGui_location Qt5::Gui LOCATION)
		add_custom_target(
				QtGui_DLL ALL
				COMMAND ${CMAKE_COMMAND} -E copy ${QtGui_location} ${CMAKE_BINARY_DIR}
		)
		get_target_property(QtWidgets_location Qt5::Widgets LOCATION)
		add_custom_target(
				QtWidgets_DLL ALL
				COMMAND ${CMAKE_COMMAND} -E copy ${QtWidgets_location} ${CMAKE_BINARY_DIR}
		)
	endif()
endif()
