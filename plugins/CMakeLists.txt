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
add_subdirectory(disk-mount)
add_subdirectory(systeminfo)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libdatetime.so
        ${CMAKE_CURRENT_BINARY_DIR}/libdisk-mount.so
        ${CMAKE_CURRENT_BINARY_DIR}/libsystem-tray.so
        ${CMAKE_CURRENT_BINARY_DIR}/libnetwork.so
        ${CMAKE_CURRENT_BINARY_DIR}/libsound.so
        ${CMAKE_CURRENT_BINARY_DIR}/libshutdown.so
        ${CMAKE_CURRENT_BINARY_DIR}/libsysteminfo.so
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/flyos-topbar/plugins)