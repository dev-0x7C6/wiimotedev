# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

#get_target_property(QtCore_location Qt5::Core LOCATION)
#get_target_property(QtDbus_location Qt5::Dbus LOCATION)
#get_target_property(QtNetwork_location Qt5::Network LOCATION)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/linux/usr/include/
)

set(sources
  ${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/consts.h
  ${CMAKE_SOURCE_DIR}/config.h
  ${CMAKE_SOURCE_DIR}/syslog/syslog.h
  ${CMAKE_SOURCE_DIR}/syslog/syslog.cpp

  daemon.cpp

  messages/classicmessages.cpp
  messages/nunchukmessages.cpp
  messages/wiimotemessages.cpp
  messages/motionplusmessages.cpp
  fancy/fancy.cpp

  wiimotedevconnection.cpp
  wiimotedevcore.cpp
  wiimotedevdevice.cpp
  wiimotedevsettings.cpp

  virtualcursor/virtualcursor.cpp
  virtualcursor/virtualcursor.h
)

set(headers
  dbus/deviceevents.h
  dbus/daemonservice.h

  wiimotedevconnection.h
  wiimotedevcore.h
  wiimotedevdevice.h
  wiimotedevsettings.h
)

add_executable(wiimotedev-daemon ${sources} ${headers} ${wiimotedev_project_files})

target_link_libraries(wiimotedev-daemon
  ${BLUETOOTH_LIBRARIES}
  Qt5::Core
  Qt5::DBus
  Qt5::Network
  libcwiid
)

install(TARGETS wiimotedev-daemon DESTINATION ${WIIMOTEDEV_SBIN_DIR})
