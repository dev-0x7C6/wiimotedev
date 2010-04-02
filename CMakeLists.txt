cmake_minimum_required (VERSION 2.6) 

project( wiimotedev-daemon )

find_package(Qt4 REQUIRED) 

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
find_package(CWiiD REQUIRED NO_POLICY_SCOPE)

include_directories(${CWIID_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(initd)
add_subdirectory(default)
add_subdirectory(dbus-1)
