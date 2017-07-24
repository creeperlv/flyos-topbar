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
find_package(Qt5X11Extras)
find_package(Qt5LinguistTools)

include_directories("/usr/include/libdtk-1.0/DBase")
include_directories("/usr/include/libdtk-1.0/DUtil")
include_directories("/usr/include/libdtk-1.0/DWidget")

include_directories(interfaces)

add_subdirectory(plugins)

set(QRC_FILES resources/resources.qrc)

set(DBUS_FILES
        plugins/power/dbus/dbusaccount.cpp
        plugins/power/dbus/dbusaccount.h
        plugins/power/dbus/dbuspower.cpp
        plugins/power/dbus/dbuspower.h)

set(SOURCE_FILES
        frame/main.cpp
        frame/mainframe.cpp
        frame/mainframe.h

        frame/controller/itemcontroller.cpp
        frame/controller/itemcontroller.h
        frame/controller/pluginloader.cpp
        frame/controller/pluginloader.h
        frame/controller/pluginscontroller.cpp
        frame/controller/pluginscontroller.h

        frame/dbus/dbusmenu.h
        frame/dbus/dbusmenu.cpp
        frame/dbus/dbusmenumanager.cpp
        frame/dbus/dbusmenumanager.h
        frame/dbus/dbusxmousearea.cpp
        frame/dbus/dbusxmousearea.h
        frame/dbus/dbusdisplay.cpp
        frame/dbus/dbusdisplay.h
        frame/dbus/dbusdock.h
        frame/dbus/dbusdock.cpp
        frame/dbus/dbusdockentry.cpp
        frame/dbus/dbusdockentry.h

        frame/item/item.cpp
        frame/item/item.h
        frame/item/pluginsitem.cpp
        frame/item/pluginsitem.h
        frame/item/stretchitem.cpp
        frame/item/stretchitem.h

        frame/util/itempopupwindow.cpp
        frame/util/itempopupwindow.h

        interfaces/constants.h
        interfaces/pluginproxyinterface.h
        interfaces/pluginsiteminterface.h widget/ImageButton.cpp widget/ImageButton.h)


file(GLOB INSTALLER_TRANSLATION_TS
        ${CMAKE_CURRENT_SOURCE_DIR}/translations/flyos-topbar*.ts)
set_source_files_properties(${INSTALLER_TRANSLATION_TS}
        PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/translations)
qt5_add_translation(INSTALLER_TRANSLATION_QM ${INSTALLER_TRANSLATION_TS})
add_custom_target(installer-qm-update DEPENDS ${INSTALLER_TRANSLATION_QM})

add_executable(flyos-topbar ${SOURCE_FILES} ${QRC_FILES})
target_link_libraries(flyos-topbar Qt5::Widgets Qt5::Gui Qt5::Core Qt5::DBus Qt5::X11Extras xcb-ewmh xcb dtkbase dtkwidget)

add_dependencies(flyos-topbar installer-qm-update)

install(TARGETS flyos-topbar RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES flyos-topbar.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/translations/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/flyos-topbar/translations
        FILES_MATCHING PATTERN "*")
