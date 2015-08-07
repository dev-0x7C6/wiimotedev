# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

include_directories(${CMAKE_CURRENT_BINARY_DIR}
		    ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/linux/usr/include/
)

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

qt5_add_resources(managersources ${resources})

qt5_wrap_ui(managersources ${manageruis})

add_executable(wiimotedev-manager ${managersources} ${managerheaders} ${managerheaders_qt})
target_link_libraries(wiimotedev-manager
  Qt5::Core
  Qt5::DBus
  Qt5::Gui
  Qt5::Widgets
)

install(TARGETS wiimotedev-manager DESTINATION  ${WIIMOTEDEV_BIN_DIR})
