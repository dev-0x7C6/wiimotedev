# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

configure_file(config.h.in
               config.h)

set(QT_DONT_USE_QTGUI TRUE)
set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTNETWORK TRUE)

include( ${QT_USE_FILE} )

add_subdirectory(include)

set(SBIN_INSTALL_DIR "${EXEC_INSTALL_PREFIX}/sbin")

set (wiimotedev_sources
        dbus/support.cpp
        dbus/deviceevents.cpp
        dbus/service.cpp
        network/support.cpp
        wiimotedev/connection.cpp
        wiimotedev/manager.cpp
        wiimotedev/settings.cpp
        wiimotedev/wiiremote.cpp
        daemon.cpp
)

set (wiimotedev_headers_qt
        dbus/deviceevents.h
        dbus/service.h
        network/support.h
        wiimotedev/connection.h
        wiimotedev/manager.h
)

set (wiimotedev_headers
        dbus/support.h
        include/wiimotedev/consts.h
        include/wiimotedev/proto.h
        wiimotedev/settings.h
        wiimotedev/wiiremote.h
)


if(USE_SYSLOG)
  list(APPEND wiimotedev_sources "syslog/syslog.cpp")
  list(APPEND wiimotedev_headers "syslog/syslog.h")
endif(USE_SYSLOG)


qt4_wrap_cpp(wiimotedev_headers_qt ${wiimotedev_headers_qt} )

add_executable(wiimotedev-daemon ${wiimotedev_sources} ${wiimotedev_headers} ${wiimotedev_headers_qt})
target_link_libraries(wiimotedev-daemon ${QT_LIBRARIES} ${CWIID_LIBRARY})

install(TARGETS wiimotedev-daemon DESTINATION ${sbin_directory})
