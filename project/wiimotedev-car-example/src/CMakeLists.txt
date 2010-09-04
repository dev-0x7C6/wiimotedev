# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTNETWORK TRUE)

include( ${QT_USE_FILE} )

set (wiimotedev_sources
        main.cpp
        car.cpp
	dbus_support.cpp
)

set (wiimotedev_headers
        car.h
	dbus_support.h
)

QT4_WRAP_CPP(wiimotedev_sources ${wiimotedev_headers} )

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(wiimotedev-car-example ${wiimotedev_sources})

target_link_libraries(wiimotedev-car-example ${QT_LIBRARIES})

install(TARGETS wiimotedev-car-example DESTINATION /usr/bin/)
