cmake_minimum_required (VERSION 2.6) 

project( wiimotedev )

find_package(Qt4 REQUIRED) 

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
find_package(CWiiD REQUIRED)

include_directories(${CWIID_INCLUDE_DIR})

set(SERVICE_INITD_DIR /etc/init.d/)
set(SERVICE_BIN_DIR /usr/sbin/)
set(SERVICE_CONF_DIR /etc/wiimotedev/)
set(SERVICE_HEADER_DIR /usr/include/)
set(SERVICE_DBUS_DIR /etc/dbus-1/system.d/)

add_subdirectory(src)
add_subdirectory(initd)
add_subdirectory(default)
add_subdirectory(dbus-1)
