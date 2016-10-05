# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008-2016 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

project(wiimotedev)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5DBus)
find_package(Qt5Widgets)

include("Version.cmake")

# Set paths
set(WIIMOTEDEV_CONF_DIR "/etc/wiimotedev")
set(WIIMOTEDEV_DBUS_DIR "/etc/dbus-1/system.d")
set(WIIMOTEDEV_SYSD_DIR "/usr/lib/systemd/system")
set(WIIMOTEDEV_INC_DIR "/usr/include")
set(WIIMOTEDEV_SBIN_DIR "/usr/sbin")
set(WIIMOTEDEV_BIN_DIR "/usr/bin/")

add_subdirectory(linux)
add_subdirectory(daemon)
add_subdirectory(io)

configure_file(config.h.in config.h)

set (system-headers
	include/wiimotedev/wiimotedev
)

install(FILES ${system-headers} DESTINATION ${WIIMOTEDEV_INC_DIR}/wiimotedev)

set(services
	systemd/wiimotedev-daemon.service
	systemd/wiimotedev-io.service
)

install(FILES ${services} DESTINATION "${WIIMOTEDEV_SYSD_DIR}"
	PERMISSIONS
	OWNER_READ  OWNER_WRITE
	GROUP_READ
	WORLD_READ
)
