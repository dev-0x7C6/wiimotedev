# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

cmake_minimum_required(VERSION 2.8)
project( wiimotedev-project )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

option(BUILD_DAEMON "Build wiimotedev-daemon, main wiiremote manager" ON)
option(BUILD_CLIENT "Build wiimotedev-client, the tcp/ip client for wiimotedev-daemon" ON)
option(BUILD_REGISTER "Build wiimotedev-register tool" ON)
option(BUILD_TOOLKIT "Build wiimotedev-toolkit tool" ON)
option(BUILD_EXAMPLES "Build additional examples" ON)

option(DAEMON_MODE "Enable daemon mode" ON)
option(USE_SYSLOG "Use syslog to raports" ON)

find_package(Qt4 REQUIRED QtCore QtDbus QtNetwork QtGui QtOpenGL)

find_package(CWiiD REQUIRED NO_POLICY_SCOPE)
find_package(Boost REQUIRED)

add_definitions(${QT_DEFINITIONS})
include(${QT_USE_FILE})

set(init_directory "/etc/init.d")
set(dbus_directory "/etc/dbus-1/system.d")
set(bin_directory "/usr/bin")
set(sbin_directory "/usr/sbin")
set(include_directory "/usr/include")

add_subdirectory(src)
add_subdirectory(init.d)
add_subdirectory(config)
add_subdirectory(dbus-1)
