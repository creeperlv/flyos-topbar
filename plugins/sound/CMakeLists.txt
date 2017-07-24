cmake_minimum_required(VERSION 3.8)
project(flyos_topbar)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
#find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets)
find_package(Qt5DBus)
find_package(Qt5Svg)

include_directories("/usr/include/libdtk-1.0/DBase")
include_directories("/usr/include/libdtk-1.0/DUtil")
include_directories("/usr/include/libdtk-1.0/DWidget")
include_directories("/usr/include/qt/QGSettings")

set(SOURCE_FILES
        sinkinputwidget.cpp
        sinkinputwidget.h
        soundapplet.h
        soundapplet.cpp
        sounditem.cpp
        sounditem.h
        soundplugin.cpp
        soundplugin.h

        resources/resources.qrc

        dbus/dbusaudio.cpp
        dbus/dbusaudio.h
        dbus/dbussink.cpp
        dbus/dbussink.h
        dbus/dbussinkinput.cpp
        dbus/dbussinkinput.h

        componments/horizontalseparator.cpp
        componments/horizontalseparator.h
        componments/volumeslider.cpp
        componments/volumeslider.h
)

add_library(sound SHARED ${SOURCE_FILES})
target_link_libraries(sound Qt5::Widgets Qt5::Gui Qt5::Core dtkwidget Qt5::DBus Qt5::Svg gsettings-qt)

install(TARGETS sound LIBRARY DESTINATION usr/lib/flyos-topbar/plugins)