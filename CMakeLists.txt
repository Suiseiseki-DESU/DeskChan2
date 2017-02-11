cmake_minimum_required (VERSION 3.6)
project(DeskChan)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

if(WIN32)
	find_library(gcclib_location NAMES gcc_s_dw2-1)
	if (gcclib_location)
		add_custom_target(
				gcclib_DLL ALL
				COMMAND ${CMAKE_COMMAND} -E copy ${gcclib_location} ${CMAKE_BINARY_DIR}
		)
	endif()
	find_library(stdcpplib_location NAMES stdc++-6)
	if (stdcpplib_location)
		add_custom_target(
				stdcpplib_DLL ALL
				COMMAND ${CMAKE_COMMAND} -E copy ${stdcpplib_location} ${CMAKE_BINARY_DIR}
		)
	endif()
	find_library(pthread_location NAMES winpthread-1)
	if (pthread_location)
		add_custom_target(
				pthread_DLL ALL
				COMMAND ${CMAKE_COMMAND} -E copy ${pthread_location} ${CMAKE_BINARY_DIR}
		)
	endif()
	get_target_property(QtCore_location Qt5::Core LOCATION)
	add_custom_target(
			QtCore_DLL ALL
			COMMAND ${CMAKE_COMMAND} -E copy ${QtCore_location} ${CMAKE_BINARY_DIR}
	)
endif()

add_subdirectory(core)
add_subdirectory(plugins)
