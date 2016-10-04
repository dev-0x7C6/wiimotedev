# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008-2016 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

project(wiimotedev)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5DBus)
find_package(Qt5Widgets)
find_package(Qt5Network)

set (wiimotedev_project_files
	${CMAKE_SOURCE_DIR}/linux/etc/dbus-1/system.d/org.wiimotedev-daemon.conf
	${CMAKE_SOURCE_DIR}/linux/etc/dbus-1/system.d/org.wiimotedev-io.conf
	${CMAKE_SOURCE_DIR}/linux/usr/lib/systemd/system/wiimotedev-daemon.service
	${CMAKE_SOURCE_DIR}/linux/usr/lib/systemd/system/wiimotedev-io.service
	${CMAKE_SOURCE_DIR}/AUTHORS
	${CMAKE_SOURCE_DIR}/CHANGELOG
	${CMAKE_SOURCE_DIR}/COPYING
	${CMAKE_SOURCE_DIR}/README
)

option(WIIMOTEDEV_BUILD_DAEMON "Build wiiremote daemon" ON)
option(WIIMOTEDEV_BUILD_IO "Build input device emulator" ON)
option(WIIMOTEDEV_INSTALL_SYSTEMD "Install systemd services" ON)

include("FindBluetooth.cmake")
include("Version.cmake")

# Set paths
set(WIIMOTEDEV_CONF_DIR "/etc/wiimotedev")
set(WIIMOTEDEV_DBUS_DIR "/etc/dbus-1/system.d")
set(WIIMOTEDEV_SYSD_DIR "/usr/lib/systemd/system")
set(WIIMOTEDEV_INC_DIR "/usr/include")
set(WIIMOTEDEV_SBIN_DIR "/usr/sbin")
set(WIIMOTEDEV_BIN_DIR "/usr/bin/")

add_subdirectory(linux)

configure_file(config.h.in config.h)

if(WIIMOTEDEV_BUILD_DAEMON)
  add_subdirectory(daemon)
endif()

if(WIIMOTEDEV_BUILD_IO)
  add_subdirectory(io)
endif(WIIMOTEDEV_BUILD_IO)


message("BUILD")
message("-- Build wiimotedev-daemon: ${WIIMOTEDEV_BUILD_DAEMON}")
message("-- Build wiimotedev-io: ${WIIMOTEDEV_BUILD_IO}")
message("-- Install systemd services: ${WIIMOTEDEV_INSTALL_SYSTEMD}")

