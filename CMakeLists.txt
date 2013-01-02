# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

cmake_minimum_required(VERSION 2.6)
project( WiimotedevProject )

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
  ${CMAKE_SOURCE_DIR}/CHANGELOG
  ${CMAKE_SOURCE_DIR}/README
)

find_package(Qt4 REQUIRED QtCore QtDbus QtNetwork QtGui QtOpenGL)

include("FindBluetooth.cmake")
include("Version.cmake")

# Set paths
set(WIIMOTEDEV_CONF_DIR "/etc/wiimotedev")
set(WIIMOTEDEV_DBUS_DIR "/etc/dbus-1/system.d")
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

add_subdirectory(examples)

if(BUILD_TOOLKIT)
  add_subdirectory(toolkit)
endif(BUILD_TOOLKIT)

message("BUILD")
message("-- Build wiimotedev-daemon: ${BUILD_DAEMON}")
message("-- Build wiimotedev-io: ${BUILD_IO}")
message("-- Build wiimotedev-toolkit: ${BUILD_TOOLKIT}")
message("-- Build wiimotedev-examples: ${BUILD_EXAMPLES}")
