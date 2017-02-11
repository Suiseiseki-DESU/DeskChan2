set(SRC_LIST Plugin.cpp Plugin.h)
add_library(python_support SHARED ${SRC_LIST})
target_link_libraries(python_support ${PROJECT_NAME}_System)

if(WIN32)
	include(ExternalProject)
	ExternalProject_Add(
			PythonBinary_Win32
			PREFIX win32
			URL https://www.python.org/ftp/python/3.6.0/python-3.6.0-embed-win32.zip
			CONFIGURE_COMMAND ""
			BUILD_COMMAND ""
			INSTALL_COMMAND ""
			SOURCE_DIR win32/bin
	)
	add_custom_target(
			pip_install ALL
			COMMAND pip install -t "${CMAKE_CURRENT_BINARY_DIR}/site-packages" aiohttp
			COMMAND python "${CMAKE_CURRENT_LIST_DIR}/package-installer.py"
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			DEPENDS PythonBinary_Win32
	)
	install(
			DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/win32/bin
			DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins/python_support/win32
	)
endif()
