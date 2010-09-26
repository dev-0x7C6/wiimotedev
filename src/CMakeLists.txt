# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

include_directories(${CMAKE_CURRENT_BINARY_DIR}
		    ${CMAKE_CURRENT_SOURCE_DIR}
		    ${CMAKE_CURRENT_SOURCE_DIR}/adaptors
		    ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/network
        ${CMAKE_SOURCE_DIR}/headers
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

set(wiimotedev_headers_qt
  adaptors/deviceevents.h
  adaptors/daemonservice.h
  network/servermanager.h
  core/connection.h
  core/manager.h
)

set(wiimotedev_headers
  adaptors/adaptors.h
  ${CMAKE_SOURCE_DIR}/headers/consts.h
  ${CMAKE_SOURCE_DIR}/headers/proto.h
  syslog/syslog.h
  core/settings.h
  core/wiiremote.h
)

set(client_sources
  adaptors/adaptors.cpp
  adaptors/deviceevents.cpp
  adaptors/daemonservice.cpp
  network/clientmanager.cpp
  syslog/syslog.cpp
  client.cpp
)

set(client_headers_qt
  adaptors/deviceevents.h
  adaptors/daemonservice.h
  network/clientmanager.h
)

set(client_headers
  adaptors/adaptors.h
  ${CMAKE_SOURCE_DIR}/headers/consts.h
  ${CMAKE_SOURCE_DIR}/headers/proto.h
  syslog/syslog.h
)

set(uinput_sources
  adaptors/customjobs.cpp
  adaptors/profilemanager.cpp
  adaptors/uinputservice.cpp
  adaptors/adaptors.cpp
  syslog/syslog.cpp
  uinput/manager.cpp
  uinput.cpp
)

set(uinput_headers_qt
  adaptors/customjobs.h
  adaptors/profilemanager.h
  adaptors/uinputservice.h
  uinput/manager.h
)

set(uinput_headers
  adaptors/adaptors.h
  ${CMAKE_SOURCE_DIR}/headers/consts.h
  ${CMAKE_SOURCE_DIR}/headers/proto.h
  syslog/syslog.h
)

if(BUILD_DAEMON)
  qt4_wrap_cpp(wiimotedev_headers_qt ${wiimotedev_headers_qt} )

  add_executable(wiimotedev-daemon ${wiimotedev_sources} ${wiimotedev_headers} ${wiimotedev_headers_qt})
  target_link_libraries(wiimotedev-daemon
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${CWIID_LIBRARY}
  )

  install(TARGETS wiimotedev-daemon DESTINATION ${sbin_directory})
endif(BUILD_DAEMON)

if(BUILD_CLIENT)
  qt4_wrap_cpp(client_sources ${client_headers_qt} )

  add_executable(wiimotedev-client ${client_sources} ${client_headers})
  target_link_libraries(wiimotedev-client
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
  )

  install(TARGETS wiimotedev-client DESTINATION ${sbin_directory})
endif(BUILD_CLIENT)

if(BUILD_UINPUT)
  qt4_wrap_cpp(uinput_sources ${uinput_headers_qt} )

  add_executable(wiimotedev-uinput ${uinput_sources} ${uinput_headers})
  target_link_libraries(wiimotedev-uinput
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
  )

  install(TARGETS wiimotedev-client DESTINATION ${sbin_directory})
endif(BUILD_UINPUT)
