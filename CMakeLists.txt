cmake_minimum_required (VERSION 3.6)
project(DeskChan)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

add_subdirectory(core)
add_subdirectory(plugins)
