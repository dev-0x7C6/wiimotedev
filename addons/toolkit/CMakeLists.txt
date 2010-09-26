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

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

set(toolkit_sources
  src/mainwindow.cpp
  src/selectwiimote.cpp
  main.cpp
)

set(toolkit_headers
  ${CMAKE_SOURCE_DIR}/headers/consts.h
)

set(toolkit_headers_qt
  ${CMAKE_SOURCE_DIR}/headers/interface.h
  src/mainwindow.h
  src/selectwiimote.h
)

set(toolkit_uis
  ui/mainwindow.ui
  ui/selectwiimote.ui
)

qt4_wrap_ui(toolkit_sources ${toolkit_uis})
qt4_wrap_cpp(toolkit_sources ${toolkit_headers_qt} )

add_executable(wiimotedev-toolkit ${toolkit_sources} ${toolkit_headers})
target_link_libraries(wiimotedev-toolkit
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

install(TARGETS wiimotedev-toolkit DESTINATION ${bin_directory})
