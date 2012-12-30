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
)

set(CMAKE_C_FLAGS "-Wall -pg ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

set(toolkit_sources
  ${CMAKE_SOURCE_DIR}/interfaces/deviceevents.cpp
  ${CMAKE_SOURCE_DIR}/interfaces/interfaces.cpp
  ${CMAKE_SOURCE_DIR}/interfaces/interfaces.h
  src/mainwindow.cpp
  src/toolkitmainwindow.cpp
  main.cpp
)

set(toolkit_headers
  ${CMAKE_SOURCE_DIR}/headers/consts.h
)

set(toolkit_headers_qt
  ${CMAKE_SOURCE_DIR}/interfaces/deviceevents.h
  src/mainwindow.h
  src/toolkitmainwindow.h
)

set(toolkit_uis
  ui/toolkitmainwindow.ui
)

set(resources
  data/data.qrc
)

qt4_add_resources(toolkit_sources ${resources})

qt4_wrap_ui(toolkit_sources ${toolkit_uis})
qt4_wrap_cpp(toolkit_sources ${toolkit_headers_qt} )

add_executable(wiimotedev-toolkit ${toolkit_sources} ${toolkit_headers})
target_link_libraries(wiimotedev-toolkit
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

install(TARGETS wiimotedev-toolkit DESTINATION  ${WIIMOTEDEV_BIN_DIR})
