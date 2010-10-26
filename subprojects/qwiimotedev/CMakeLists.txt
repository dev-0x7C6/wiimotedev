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
                    ${QTSINGLEAPPLICATION_INCLUDE_DIRS}
)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

add_subdirectory(icons)

set(qwiimotedev_sources
  ${CMAKE_SOURCE_DIR}/src/interfaces/customjobs.cpp
  ${CMAKE_SOURCE_DIR}/src/interfaces/interfaces.cpp
  ${CMAKE_SOURCE_DIR}/src/interfaces/interfaces.h
  ${CMAKE_SOURCE_DIR}/src/interfaces/deviceevents.cpp
  ${CMAKE_SOURCE_DIR}/src/interfaces/profilemanager.cpp
  src/mainwindow.cpp
  main.cpp
)

set(qwiimotedev_headers
  ${CMAKE_SOURCE_DIR}/src/interfaces/customjobs.h
  ${CMAKE_SOURCE_DIR}/src/interfaces/deviceevents.h
  ${CMAKE_SOURCE_DIR}/src/interfaces/profilemanager.h
  src/mainwindow.h

)

set(qwiimotedev_uis
  ui/mainwindow.ui
)

qt4_wrap_ui(qwiimotedev_sources ${qwiimotedev_uis})
qt4_wrap_cpp(qwiimotedev_sources ${qwiimotedev_headers})

add_executable(qwiimotedev ${qwiimotedev_sources})

add_dependencies(qwiimotedev qtsingleapplication)

target_link_libraries(qwiimotedev
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QTSINGLEAPPLICATION_LIBRARIES}
)

install(TARGETS qwiimotedev DESTINATION ${WIIMOTEDEV_BIN_DIR})
