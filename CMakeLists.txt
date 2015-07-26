# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008-2015 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

cmake_minimum_required(VERSION 2.8.11)

project( WiimotedevProject )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5DBus)
find_package(Qt5Widgets)
find_package(Qt5Network)

set(CMAKE_C_FLAGS "-march=native -O2 -Wall")
set(CMAKE_CXX_FLAGS "-march=native -O2 -std=c++14 -Wall")

set (wiimotedev_project_files
  ${CMAKE_SOURCE_DIR}/linux/etc/dbus-1/system.d/org.wiimotedev-daemon.conf
  ${CMAKE_SOURCE_DIR}/linux/etc/dbus-1/system.d/org.wiimotedev-io.conf
  ${CMAKE_SOURCE_DIR}/linux/etc/init.d/gentoo/wiimotedev-daemon
  ${CMAKE_SOURCE_DIR}/linux/etc/init.d/debian/wiimotedev-daemon
  ${CMAKE_SOURCE_DIR}/linux/etc/init.d/ubuntu/wiimotedev-daemon
  ${CMAKE_SOURCE_DIR}/linux/etc/init.d/archlinux/wiimotedev-daemon
  ${CMAKE_SOURCE_DIR}/linux/etc/init.d/slackware/wiimotedev-daemon
  ${CMAKE_SOURCE_DIR}/linux/etc/init.d/gentoo/wiimotedev-io
  ${CMAKE_SOURCE_DIR}/linux/etc/init.d/debian/wiimotedev-io
  ${CMAKE_SOURCE_DIR}/linux/etc/init.d/ubuntu/wiimotedev-io
  ${CMAKE_SOURCE_DIR}/linux/etc/init.d/archlinux/wiimotedev-io
  ${CMAKE_SOURCE_DIR}/linux/etc/init.d/slackware/wiimotedev-io
  ${CMAKE_SOURCE_DIR}/linux/usr/lib/systemd/system/wiimotedev-daemon.service
  ${CMAKE_SOURCE_DIR}/linux/usr/lib/systemd/system/wiimotedev-daemon.target
  ${CMAKE_SOURCE_DIR}/linux/usr/lib/systemd/system/wiimotedev-io.service
  ${CMAKE_SOURCE_DIR}/linux/usr/lib/systemd/system/wiimotedev-io.target
  ${CMAKE_SOURCE_DIR}/CHANGELOG
  ${CMAKE_SOURCE_DIR}/README
)

option(INSTALL_SYSTEMD "Install systemd services" ON)
option(INSTALL_INITD "Install init.d services")
option(BUILD_DAEMON "Build wiimotedev-daemon, main wiiremote manager" ON)
option(BUILD_IO "Build wiimotedev-io, I/O device emulator" ON)
option(BUILD_MANAGER "Build wiimotedev-manager tool" ON)

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
add_subdirectory(static)

configure_file(config.h.in
               config.h)

if(BUILD_DAEMON)
  add_subdirectory(daemon)
endif()

if(BUILD_IO)
  add_subdirectory(io)
endif(BUILD_IO)

if(BUILD_MANAGER)
  add_subdirectory(manager)
endif(BUILD_MANAGER)

message("BUILD")
message("-- Install systemd services: ${INSTALL_SYSTEMD}")
message("-- Install init.d services: ${INSTALL_INITD}")
message("-- Build wiimotedev-daemon: ${BUILD_DAEMON}")
message("-- Build wiimotedev-io: ${BUILD_IO}")
message("-- Build wiimotedev-manager: ${BUILD_MANAGER}")
