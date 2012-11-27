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

if(USE_STATIC_CWIID)
  include_directories(${CWIID_INCLUDE_DIR})
endif(USE_STATIC_CWIID)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

set(wiimotedev_sources
  ${CMAKE_SOURCE_DIR}/dbus/adaptors/adaptors.h
  ${CMAKE_SOURCE_DIR}/headers/consts.h
  ${CMAKE_SOURCE_DIR}/syslog/syslog.h
  ${CMAKE_SOURCE_DIR}/syslog/syslog.cpp

  ${CMAKE_SOURCE_DIR}/dbus/adaptors/deviceevents.cpp
  ${CMAKE_SOURCE_DIR}/dbus/adaptors/daemonservice.cpp

  daemon/wiimotemanager.cpp
  daemon/settings.cpp
  daemon/wiimotedevice.cpp
  daemon/wiimotemessagethread.cpp
  daemon/classicmessages.cpp
  daemon/nunchukmessages.cpp
  daemon/wiimotemessages.cpp
  daemon/virtualcursor.cpp
  daemon/virtualcursor.h
  daemon/features.cpp
  daemon.cpp
)

set(wiimotedev_headers
  ${CMAKE_SOURCE_DIR}/dbus/adaptors/deviceevents.h
  ${CMAKE_SOURCE_DIR}/dbus/adaptors/daemonservice.h
  daemon/wiimotemessagethread.h
  daemon/wiimotemanager.h
  daemon/settings.h
  daemon/wiimotedevice.h
)

qt4_wrap_cpp(wiimotedev_sources ${wiimotedev_headers})

add_executable(wiimotedev-daemon ${wiimotedev_sources})
target_link_libraries(wiimotedev-daemon
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${CWIID_LIBRARY}
)

install(TARGETS wiimotedev-daemon DESTINATION ${WIIMOTEDEV_SBIN_DIR})
