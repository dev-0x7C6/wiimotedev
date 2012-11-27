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
                    ${QT_QTOPENGL_INCLUDE_DIR}
        ${QTSINGLEAPPLICATION_INCLUDE_DIRS}
)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

set(manager_sources
  ${CMAKE_SOURCE_DIR}/interfaces/customjobs.cpp
  ${CMAKE_SOURCE_DIR}/interfaces/interfaces.cpp
  ${CMAKE_SOURCE_DIR}/interfaces/interfaces.h
  ${CMAKE_SOURCE_DIR}/interfaces/deviceevents.cpp
  ${CMAKE_SOURCE_DIR}/interfaces/profilemanager.cpp
  widgets/graphicsprofilecover.cpp
  widgets/graphicsbutton.cpp
  widgets/graphicscheckbox.cpp
  widgets/graphicsmanagermenu.cpp
  src/mainwindow.cpp
  main.cpp
)

set(manager_headers
  ${CMAKE_SOURCE_DIR}/interfaces/customjobs.h
  ${CMAKE_SOURCE_DIR}/interfaces/deviceevents.h
  ${CMAKE_SOURCE_DIR}/interfaces/profilemanager.h
  widgets/graphicsbutton.h
  widgets/graphicsprofilecover.h
  widgets/graphicscheckbox.h
  widgets/graphicsmanagermenu.h
  src/mainwindow.h
)

set(manager_uis
)

set(resources
  data/data.qrc
)


qt4_add_resources(manager_sources ${resources})

qt4_wrap_ui(manager_sources ${manager_uis})
qt4_wrap_cpp(manager_sources ${manager_headers})

add_executable(wiimotedev-manager ${manager_sources})

add_dependencies(wiimotedev-manager qtsingleapplication)

target_link_libraries(wiimotedev-manager
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTNETWORK_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTOPENGL_LIBRARY}
  ${QTSINGLEAPPLICATION_LIBRARIES}
)

install(TARGETS wiimotedev-manager DESTINATION ${WIIMOTEDEV_BIN_DIR})
