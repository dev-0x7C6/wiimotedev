# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

include_directories(${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${QT_INCLUDE_DIR}
  ${QT_QTCORE_INCLUDE_DIR}
  ${QT_QTGUI_INCLUDE_DIR}
  ${QT_QTDBUS_INCLUDE_DIR}
)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

set(sources
${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/consts.h
${CMAKE_SOURCE_DIR}/adaptors/adaptors.h
${CMAKE_SOURCE_DIR}/syslog/syslog.cpp

 eiobase/eiouinputobject.cpp
 eiobase/eiomousedevice.cpp
 eiobase/eioeventdevice.cpp
 eiobase/eiotouchscreen.cpp

 eiojoystick/eioclassicjoystick.cpp
 eiojoystick/eionunchukjoystick.cpp
 eiojoystick/eiowiimotejoystick.cpp
 
  eiomanager/infrared.cpp
  eiomanager/keyboard.cpp
  eiomanager/manager.cpp
  eiomanager/actions.cpp
  eiomanager/eiojoystickassign.cpp

  eiokeyboard/eioremotekeyboard.cpp
  eioinfrared/eioinfraredmouse.cpp
  uinput.cpp

)

set(headers
  ${CMAKE_SOURCE_DIR}/syslog/syslog.h

  eiobase/eiouinputobject.h
  eiobase/eiomousedevice.h
  eiobase/eioeventdevice.h
  eiobase/eiotouchscreen.h
)

set(headers_wrap
  eiojoystick/eioclassicjoystick.h
  eiojoystick/eionunchukjoystick.h
  eiojoystick/eiowiimotejoystick.h
  helper/hashcompare.h

  ${CMAKE_SOURCE_DIR}/adaptors/customjobs.h
  ${CMAKE_SOURCE_DIR}/adaptors/profilemanager.h
  ${CMAKE_SOURCE_DIR}/adaptors/uinputservice.h
  ${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/deviceevents.h
  eiomanager/manager.h
  eiokeyboard/eioremotekeyboard.h
  eioinfrared/eioinfraredmouse.h
)


qt4_wrap_cpp(headers ${headers_wrap})

add_executable(wiimotedev-io ${headers_wrap} ${headers} ${sources})
target_link_libraries(wiimotedev-io
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
)

install(TARGETS wiimotedev-io DESTINATION ${WIIMOTEDEV_SBIN_DIR})
