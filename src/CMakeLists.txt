# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

include_directories(${CMAKE_CURRENT_BINARY_DIR}
		    ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/adaptors
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
  wiimotedev/connection.cpp
  wiimotedev/manager.cpp
  wiimotedev/settings.cpp
  wiimotedev/wiiremote.cpp
  daemon.cpp
)

set(wiimotedev_headers_qt
  adaptors/deviceevents.h
  adaptors/daemonservice.h
  network/servermanager.h
  wiimotedev/connection.h
  wiimotedev/manager.h
)

set(wiimotedev_headers
  adaptors/adaptors.h
  ${CMAKE_SOURCE_DIR}/headers/consts.h
  ${CMAKE_SOURCE_DIR}/headers/proto.h
  syslog/syslog.h
  wiimotedev/settings.h
  wiimotedev/wiiremote.h
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
  uinput.cpp
)

set(uinput_headers_qt
  adaptors/customjobs.h
  adaptors/profilemanager.h
  adaptors/uinputservice.h
)

set(uinput_headers
  adaptors/adaptors.h
  ${CMAKE_SOURCE_DIR}/headers/consts.h
  ${CMAKE_SOURCE_DIR}/headers/proto.h
  syslog/syslog.h
)

set(register_sources
  register.cpp
)

set(register_headers
  ${CMAKE_SOURCE_DIR}/headers/consts.h
)


set(toolkit_sources
  toolkit/mainwindow.cpp
  toolkit/selectwiimote.cpp
  toolkit.cpp
)

set(toolkit_headers
  ${CMAKE_SOURCE_DIR}/headers/consts.h
)

set(toolkit_headers_qt
  ${CMAKE_SOURCE_DIR}/headers/interface.h
  toolkit/mainwindow.h
  toolkit/selectwiimote.h
)

set(toolkit_uis
  toolkit/mainwindow.ui
  toolkit/selectwiimote.ui
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

if(BUILD_REGISTER)
  add_executable(wiimotedev-register ${register_sources} ${register_headers})
  target_link_libraries(wiimotedev-register
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
  )

  install(TARGETS wiimotedev-register DESTINATION ${bin_directory})
endif(BUILD_REGISTER)

if(BUILD_TOOLKIT)
  qt4_wrap_ui(toolkit_sources ${toolkit_uis})
  qt4_wrap_cpp(toolkit_sources ${toolkit_headers_qt} )

  add_executable(wiimotedev-toolkit ${toolkit_sources} ${toolkit_headers})
  target_link_libraries(wiimotedev-toolkit
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTGUI_LIBRARY}
  )

  install(TARGETS wiimotedev-toolkit DESTINATION ${bin_directory})
endif(BUILD_TOOLKIT)
