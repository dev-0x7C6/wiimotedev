# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

include_directories(${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${QT_INCLUDE_DIR}
  ${QT_QTCORE_INCLUDE_DIR}
  ${QT_QTDBUS_INCLUDE_DIR}
)

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

set(sources
${CMAKE_SOURCE_DIR}/headers/consts.h
${CMAKE_SOURCE_DIR}/dbus/adaptors/adaptors.h
${CMAKE_SOURCE_DIR}/syslog/syslog.cpp

 eiobase/eiouinputobject.cpp
 eiobase/eiomousedevice.cpp
 eiobase/eioeventdevice.cpp
 eiobase/eiotouchscreen.cpp

 eiojoystick/eioclassicjoystick.cpp
 eiojoystick/eionunchukjoystick.cpp
 eiojoystick/eiowiimotejoystick.cpp
 

  ${CMAKE_SOURCE_DIR}/dbus/adaptors/customjobs.cpp
  ${CMAKE_SOURCE_DIR}/dbus/adaptors/profilemanager.cpp
  ${CMAKE_SOURCE_DIR}/dbus/adaptors/uinputservice.cpp
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/deviceevents.cpp
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/interfaces.cpp

  eiomanager/infrared.cpp
  eiomanager/keyboard.cpp
  eiomanager/manager.cpp
  eiomanager/actions.cpp
  eiomanager/eiojoystickassign.cpp
  virtual/mouse/infrared.cpp
  virtual/event/keyboard.cpp
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

  ${CMAKE_SOURCE_DIR}/dbus/adaptors/customjobs.h
  ${CMAKE_SOURCE_DIR}/dbus/adaptors/profilemanager.h
  ${CMAKE_SOURCE_DIR}/dbus/adaptors/uinputservice.h
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/interfaces.h
  ${CMAKE_SOURCE_DIR}/dbus/interfaces/deviceevents.h
  eiomanager/manager.h
  virtual/mouse/infrared.h
  virtual/event/keyboard.h
)


qt4_wrap_cpp(headers ${headers_wrap})

add_executable(wiimotedev-uinput ${headers} ${sources})
target_link_libraries(wiimotedev-uinput
  ${QT_QTCORE_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
)

install(TARGETS wiimotedev-uinput DESTINATION ${WIIMOTEDEV_SBIN_DIR})
