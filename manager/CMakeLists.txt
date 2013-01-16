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
)

set(CMAKE_C_FLAGS "-Wall -pg ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

set(managersources
  src/infraredcameraview.cpp
  src/wiimoterawstream.cpp
  src/virtualgamepads.cpp
  src/mainwindow.cpp
  main.cpp
)

set(managerheaders
  ${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/consts.h
)

set(managerheaders_qt
  ${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/deviceevents.h
  src/infraredcameraview.h
  src/wiimoterawstream.h
  src/virtualgamepads.h
  src/mainwindow.h
)

set(manageruis
  ui/wiimoterawstream.ui
  ui/mainwindow.ui
  ui/virtualgamepads.ui
)

set(resources
  data/data.qrc
)

qt4_add_resources(managersources ${resources})

qt4_wrap_ui(managersources ${manageruis})
qt4_wrap_cpp(managersources ${managerheaders_qt} )

add_executable(wiimotedev-manager ${managersources} ${managerheaders})
target_link_libraries(wiimotedev-manager
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

install(TARGETS wiimotedev-manager DESTINATION  ${WIIMOTEDEV_BIN_DIR})
