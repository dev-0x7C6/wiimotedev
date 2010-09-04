# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTNETWORK TRUE)
set(QT_USE_QT3SUPPORT TRUE)

include( ${QT_USE_FILE} )

set (wiimotedev_sources
        ui/selectwiimote.cpp
        main.cpp
	mainwindow.cpp
)

set (wiimotedev_headers
        ui/selectwiimote.h
	interface.h
	mainwindow.h
)

set (wiimotedev_uis
        ui/selectwiimote.ui
        mainwindow.ui
)


QT4_WRAP_UI(wiimotedev_sources ${wiimotedev_uis})
QT4_WRAP_CPP(wiimotedev_sources ${wiimotedev_headers} )

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(wiimotedev-toolkit ${wiimotedev_sources})

target_link_libraries(wiimotedev-toolkit ${QT_LIBRARIES})

install(TARGETS wiimotedev-toolkit DESTINATION /usr/bin/)
