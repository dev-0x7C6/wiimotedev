# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

cmake_minimum_required(VERSION 2.6)
project( WiimotedevProject )

find_package(Qt4 REQUIRED QtCore QtDbus QtNetwork QtGui QtOpenGL)

include("cmake/FindBluetooth.cmake")
include("cmake/Version.cmake")

# Set paths
set(WIIMOTEDEV_CONF_DIR "/etc/wiimotedev")
set(WIIMOTEDEV_DBUS_DIR "/etc/dbus-1/system.d")
set(WIIMOTEDEV_INC_DIR "/usr/include")
set(WIIMOTEDEV_SBIN_DIR "/usr/sbin")

# Additional projects can be installed in directories such as /usr/local/
set(WIIMOTEDEV_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin/")

option(USE_STATIC_CWIID "Use static build-in cwiid library" ON)

if(USE_STATIC_CWIID)
  set(CWIID_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libcwiid)
  set(CWIID_LIBRARY
    libcwiid
    ${BLUETOOTH_LIBRARIES}
  )
else(USE_STATIC_CWIID)
  include("cmake/FindCWiiD.cmake")
endif(USE_STATIC_CWIID)

set(QTSINGLEAPPLICATION_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qtsingleapplication)
set(QTSINGLEAPPLICATION_LIBRARIES qtsingleapplication)

add_subdirectory(3rdparty)

option(BUILD_DAEMON "Build wiimotedev-daemon, main wiiremote manager" ON)
option(BUILD_UINPUT "Build wiimotedev-uinput, input device emulator" ON)
option(BUILD_REGISTER "Build wiimotedev-register tool" ON)
option(BUILD_TOOLKIT "Build wiimotedev-toolkit tool" ON)
option(BUILD_EXAMPLES "Build additional examples" ON)
option(BUILD_QWIIMOTEDEV "Build qwiimotedev, front-end for wiimotedev-uinput" ON)

add_subdirectory(config)
add_subdirectory(dbus-1)
add_subdirectory(headers)
add_subdirectory(src)

add_subdirectory(examples)
add_subdirectory(subprojects)

message("")
message("===========================================")
message("BUILD")
message("-- Build wiimotedev-daemon: ${BUILD_DAEMON}")
message("-- Build wiimotedev-uinput: ${BUILD_UINPUT}")
message("")
message("-- Build register: ${BUILD_REGISTER}")
message("-- Build toolkit: ${BUILD_DAEMON}")
message("-- Build examples: ${BUILD_EXAMPLES}")
message("-- Build qwiimotedev: ${BUILD_QWIIMOTEDEV}")
message("")
message("INSTALATION")
message("-- Installation path: ${CMAKE_INSTALL_PREFIX}")
add_subdirectory(runscripts)
message("===========================================")
message("")
