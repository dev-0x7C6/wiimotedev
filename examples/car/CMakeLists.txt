# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

cmake_minimum_required(VERSION 2.6)
project( WiimotedevProjectExamples )

find_package(Qt4 REQUIRED QtCore QtDbus QtNetwork QtGui QtOpenGL)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${QT_INCLUDE_DIR}
        ${QT_QTCORE_INCLUDE_DIR}
        ${QT_QTDBUS_INCLUDE_DIR}
        ${QT_QTGUI_INCLUDE_DIR}
)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

set(car_example_sources
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/deviceevents.cpp
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/interfaces.cpp
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/interfaces.h
  car.cpp
  main.cpp
)

set(car_example_headers
  ${CMAKE_SOURCE_DIR}/headers/consts.h
)

set(car_example_headers_qt
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/deviceevents.h
  car.h
)

qt4_wrap_cpp(car_example_sources ${car_example_headers_qt} )

add_executable(wiimotedev-car-example ${car_example_sources} ${car_example_headers})
target_link_libraries(wiimotedev-car-example
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

set(DIRECTORY "${CMAKE_INSTALL_PREFIX}/share/wiimotedev/examples/car/")
set(SOURCE_COPY
  CMakeLists.txt
  car.cpp
  car.h
  main.cpp
)

set(IFACES_COPY
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/deviceevents.cpp
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/deviceevents.h
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/interfaces.cpp
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/interfaces.h
)

set(HEADER_COPY
  ${CMAKE_SOURCE_DIR}/headers/consts.h
)

install(FILES ${SOURCE_COPY} DESTINATION ${DIRECTORY} )
install(FILES ${IFACES_COPY} DESTINATION ${DIRECTORY}/dbus/interfaces )
install(FILES ${HEADER_COPY} DESTINATION ${DIRECTORY}/headers/ )

install(TARGETS wiimotedev-car-example DESTINATION ${DIRECTORY})
