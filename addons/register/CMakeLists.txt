# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

include_directories(${CMAKE_CURRENT_BINARY_DIR}
		    ${CMAKE_CURRENT_SOURCE_DIR}
		    ${CMAKE_CURRENT_SOURCE_DIR}/src
		    ${CMAKE_SOURCE_DIR}/headers
		    ${QT_INCLUDE_DIR}
		    ${QT_QTCORE_INCLUDE_DIR}
		    ${QT_QTDBUS_INCLUDE_DIR}
		    ${QT_QTGUI_INCLUDE_DIR}
)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

set(register_sources
  src/main.cpp
)

set(register_headers
  ${CMAKE_SOURCE_DIR}/headers/consts.h
)

add_executable(wiimotedev-register ${register_sources} ${register_headers})
target_link_libraries(wiimotedev-register
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
)

install(TARGETS wiimotedev-register DESTINATION ${bin_directory})