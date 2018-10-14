project(wiimotedev)
cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5DBus)
find_package(Qt5Widgets)

set(WIIMOTEDEV_VERSION_MAJOR 2)
set(WIIMOTEDEV_VERSION_MINOR 9)
set(WIIMOTEDEV_VERSION_PATCH 0)

configure_file(config.h.in config.h)
add_subdirectory(daemon)
add_subdirectory(dbus-1/conf)
add_subdirectory(dbus-1/interfaces)
add_subdirectory(include)
add_subdirectory(io)
add_subdirectory(services)
