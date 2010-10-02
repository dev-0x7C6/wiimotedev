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

message(${CMAKE_SOURCE_DIR})

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

set(car_example_sources
  ${CMAKE_SOURCE_DIR}/src/interfaces/deviceevents.cpp
  ${CMAKE_SOURCE_DIR}/src/interfaces/interfaces.cpp
  ${CMAKE_SOURCE_DIR}/src/interfaces/interfaces.h
  car.cpp
  main.cpp
)

set(car_example_headers
  ${CMAKE_SOURCE_DIR}/headers/consts.h
)

set(car_example_headers_qt
  ${CMAKE_SOURCE_DIR}/src/interfaces/deviceevents.h
  car.h
)

qt4_wrap_cpp(car_example_sources ${car_example_headers_qt} )

add_executable(wiimotedev-car-example ${car_example_sources} ${car_example_headers})
target_link_libraries(wiimotedev-car-example
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

install(TARGETS wiimotedev-car-example DESTINATION ${bin_directory})
