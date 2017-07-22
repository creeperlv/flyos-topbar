cmake_minimum_required(VERSION 3.8)
project(plugins)

set(CMAKE_CXX_STANDARD 14)

include_directories(interfaces)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/)

add_subdirectory(datetime)
add_subdirectory(shutdown)
add_subdirectory(system-tray)
add_subdirectory(sound)
add_subdirectory(network)
