# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

cmake_minimum_required(VERSION 2.6)
project( wiimotedev-daemon )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Qt4 REQUIRED QtCore)
find_package(Qt4 REQUIRED QtDbus)
find_package(CWiiD REQUIRED NO_POLICY_SCOPE)

option(DAEMON_MODE "Enable daemon mode" ON)
option(USE_SYSLOG "Use syslog to raports" ON)

set(init_directory "/etc/init.d")
set(dbus_directory "/etc/dbus-1/system.d")
set(sbin_directory "/usr/sbin")
set(include_directory "/usr/include")

add_subdirectory(src)
add_subdirectory(init.d)
add_subdirectory(config)
add_subdirectory(dbus-1)
