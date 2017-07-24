cmake_minimum_required(VERSION 3.8)
project(flyos_topbar)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5DBus)
find_package(Qt5Svg)
find_package(Qt5X11Extras)

include_directories("/usr/include/libdtk-1.0/DBase")
include_directories("/usr/include/libdtk-1.0/DUtil")
include_directories("/usr/include/libdtk-1.0/DWidget")

set(SOURCE_FILES
        systemtrayplugin.cpp
        systemtrayplugin.h
        tipswidget.cpp
        tipswidget.h
        traywidget.cpp
        traywidget.h

        dbus/dbustraymanager.h
        dbus/dbustraymanager.cpp)



add_library(system-tray SHARED ${SOURCE_FILES})
target_link_libraries(system-tray Qt5::Widgets Qt5::Gui Qt5::Core dtkwidget Qt5::DBus Qt5::Svg Qt5::X11Extras xcb xcb-image xcb-icccm xcb-composite Xtst)
