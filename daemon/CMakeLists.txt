# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

include_directories(${CMAKE_CURRENT_BINARY_DIR}
		    ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/linux/usr/include/
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
  ${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/consts.h
  ${CMAKE_SOURCE_DIR}/syslog/syslog.h
  ${CMAKE_SOURCE_DIR}/syslog/syslog.cpp

  wiimotemanager.cpp
  settings.cpp
  wiimotedevice.cpp
  wiimotemessagethread.cpp
  messages/classicmessages.cpp
  messages/nunchukmessages.cpp
  messages/wiimotemessages.cpp
  virtualcursor/virtualcursor.cpp
  virtualcursor/virtualcursor.h
  features.cpp
  daemon.cpp

  ${CMAKE_SOURCE_DIR}/config.h
)

set(headers
  dbus/deviceevents.h
  dbus/daemonservice.h

  wiimotemessagethread.h
  wiimotemanager.h
  settings.h
  wiimotedevice.h
)

qt4_wrap_cpp(sources ${headers})

add_executable(wiimotedev-daemon ${sources} ${headers} ${wiimotedev_project_files})
target_link_libraries(wiimotedev-daemon
  ${BLUETOOTH_LIBRARIES}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  libcwiid
)

install(TARGETS wiimotedev-daemon DESTINATION ${WIIMOTEDEV_SBIN_DIR})
