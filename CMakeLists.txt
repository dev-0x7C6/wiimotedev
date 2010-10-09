# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

cmake_minimum_required(VERSION 2.6)
project( WiimotedevProject )

find_package(Qt4 REQUIRED QtCore QtDbus QtNetwork QtGui QtOpenGL)

include("cmake/FindCWiiD.cmake")
include("cmake/Version.cmake")

option(BUILD_DAEMON "Build wiimotedev-daemon, main wiiremote manager" ON)
option(BUILD_CLIENT "Build wiimotedev-client, the tcp/ip client for wiimotedev-daemon" ON)
option(BUILD_UINPUT "Build wiimotedev-uinput, input device emulator" ON)
option(BUILD_REGISTER "Build wiimotedev-register tool" ON)
option(BUILD_TOOLKIT "Build wiimotedev-toolkit tool" ON)
option(BUILD_EXAMPLES "Build additional examples" ON)
option(BUILD_QWIIMOTEDEV "Build qwiimotedev, front-end for wiimotedev-uinput" ON)

set(init_directory "/etc/init.d")
set(dbus_directory "/etc/dbus-1/system.d")
set(bin_directory "${CMAKE_INSTALL_PREFIX}/bin")
set(sbin_directory "${CMAKE_INSTALL_PREFIX}/sbin")
set(include_directory "${CMAKE_INSTALL_PREFIX}/include")

add_subdirectory(addons)
add_subdirectory(config)
add_subdirectory(dbus-1)
add_subdirectory(examples)
add_subdirectory(headers)
add_subdirectory(src)

message("")
message("===========================================")
message("BUILD")
message("-- Build wiimotedev-daemon: ${BUILD_DAEMON}")
message("-- Build wiimotedev-client: ${BUILD_CLIENT}")
message("-- Build wiimotedev-uinput: ${BUILD_UINPUT}")
message("")
message("-- Build register: ${BUILD_REGISTER}")
message("-- Build toolkit: ${BUILD_DAEMON}")
message("-- Build examples: ${BUILD_CLIENT}")
message("-- Build qwiimotedev: ${BUILD_QWIIMOTEDEV}")
message("")
message("INSTALATION")
message("-- Installation path: ${CMAKE_INSTALL_PREFIX}")
add_subdirectory(runscripts)
message("===========================================")
message("")
