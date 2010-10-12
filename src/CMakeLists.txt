# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

include_directories(${CMAKE_CURRENT_BINARY_DIR}
		    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}
		    ${QT_INCLUDE_DIR}
		    ${QT_QTCORE_INCLUDE_DIR}
		    ${QT_QTDBUS_INCLUDE_DIR}
		    ${QT_QTGUI_INCLUDE_DIR}
		    ${QT_QTNETWORK_INCLUDE_DIR}
)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

configure_file(config.h.in
               config.h)

set(wiimotedev_sources
  adaptors/adaptors.h
  core/settings.h
  core/wiiremote.h
  ${CMAKE_SOURCE_DIR}/headers/consts.h
  ${CMAKE_SOURCE_DIR}/headers/proto.h
  syslog/syslog.h

  adaptors/adaptors.cpp
  adaptors/deviceevents.cpp
  adaptors/daemonservice.cpp
  network/servermanager.cpp
  syslog/syslog.cpp
  core/connection.cpp
  core/manager.cpp
  core/settings.cpp
  core/wiiremote.cpp
  daemon.cpp
)

set(wiimotedev_headers
  adaptors/deviceevents.h
  adaptors/daemonservice.h
  network/servermanager.h
  core/connection.h
  core/manager.h
)

set(client_sources
  adaptors/adaptors.h
  ${CMAKE_SOURCE_DIR}/headers/consts.h
  ${CMAKE_SOURCE_DIR}/headers/proto.h
  syslog/syslog.h

  adaptors/adaptors.cpp
  adaptors/deviceevents.cpp
  adaptors/daemonservice.cpp
  network/clientmanager.cpp
  syslog/syslog.cpp
  client.cpp
)

set(client_headers
  adaptors/deviceevents.h
  adaptors/daemonservice.h
  network/clientmanager.h
)

set(uinput_sources
  adaptors/adaptors.h
  classes/hashcompare.h
  devices/classicgamepad.h
  devices/general.h
  devices/mouse.h
  devices/eventdevice.h
  devices/touchscreen.h
  devices/wiimotegamepad.h
  interfaces/interfaces.h
  ${CMAKE_SOURCE_DIR}/headers/consts.h
  ${CMAKE_SOURCE_DIR}/headers/proto.h
  syslog/syslog.h

  adaptors/customjobs.cpp
  adaptors/profilemanager.cpp
  adaptors/uinputservice.cpp
  adaptors/adaptors.cpp
  devices/classicgamepad.cpp
  devices/general.cpp
  devices/mouse.cpp
  devices/eventdevice.cpp
  devices/touchscreen.cpp
  devices/wiimotegamepad.cpp
  interfaces/deviceevents.cpp
  interfaces/interfaces.cpp
  syslog/syslog.cpp
  uinput/gamepads.cpp
  uinput/infrared.cpp
  uinput/keyboard.cpp
  uinput/manager.cpp
  uinput/actions.cpp
  uinput.cpp
)

set(uinput_headers
  adaptors/customjobs.h
  adaptors/profilemanager.h
  adaptors/uinputservice.h
  interfaces/deviceevents.h
  uinput/manager.h
)

if(BUILD_DAEMON)
  qt4_wrap_cpp(wiimotedev_sources ${wiimotedev_headers})

  add_executable(wiimotedev-daemon ${wiimotedev_sources})
  target_link_libraries(wiimotedev-daemon
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${CWIID_LIBRARY}
  )

  install(TARGETS wiimotedev-daemon DESTINATION ${sbin_directory})
endif(BUILD_DAEMON)

if(BUILD_CLIENT)
  qt4_wrap_cpp(client_sources ${client_headers} )

  add_executable(wiimotedev-client ${client_sources})
  target_link_libraries(wiimotedev-client
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
  )

  install(TARGETS wiimotedev-client DESTINATION ${sbin_directory})
endif(BUILD_CLIENT)

if(BUILD_UINPUT)
  qt4_wrap_cpp(uinput_sources ${uinput_headers})

  add_executable(wiimotedev-uinput ${uinput_sources})
  target_link_libraries(wiimotedev-uinput
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
  )

  install(TARGETS wiimotedev-uinput DESTINATION ${sbin_directory})
endif(BUILD_UINPUT)
