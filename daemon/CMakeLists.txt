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

set(sources
  ${CMAKE_SOURCE_DIR}/adaptors/adaptors.h
  ${CMAKE_SOURCE_DIR}/headers/consts.h
  ${CMAKE_SOURCE_DIR}/syslog/syslog.h
  ${CMAKE_SOURCE_DIR}/syslog/syslog.cpp

  ${CMAKE_SOURCE_DIR}/adaptors/deviceevents.cpp
  ${CMAKE_SOURCE_DIR}/adaptors/daemonservice.cpp

  wiimotemanager.cpp
  settings.cpp
  wiimotedevice.cpp
  wiimotemessagethread.cpp
  classicmessages.cpp
  nunchukmessages.cpp
  wiimotemessages.cpp
  virtualcursor.cpp
  virtualcursor.h
  features.cpp
  daemon.cpp
)

set(headers
  ${CMAKE_SOURCE_DIR}/adaptors/deviceevents.h
  ${CMAKE_SOURCE_DIR}/adaptors/daemonservice.h
  wiimotemessagethread.h
  wiimotemanager.h
  settings.h
  wiimotedevice.h
)

qt4_wrap_cpp(sources ${headers})

add_executable(wiimotedev-daemon ${sources})
target_link_libraries(wiimotedev-daemon
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${CWIID_LIBRARY}
)

install(TARGETS wiimotedev-daemon DESTINATION ${WIIMOTEDEV_SBIN_DIR})
