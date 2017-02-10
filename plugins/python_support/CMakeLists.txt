find_package(PythonLibs)

if(PythonLibs_FOUND)

	include_directories(${PYTHON_INCLUDE_DIRS})

	set(SRC_LIST Plugin.cpp Plugin.h PythonPlugin.cpp PythonPlugin.h)
	add_library(python_support SHARED ${SRC_LIST})
	target_link_libraries(python_support ${PYTHON_LIBRARIES} ${PROJECT_NAME}_System)

endif()
