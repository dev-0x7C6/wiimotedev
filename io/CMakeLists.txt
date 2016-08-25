# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

include_directories(${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/linux/usr/include/
)

set(sources
  ${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/consts.h
  ${CMAKE_SOURCE_DIR}/syslog/syslog.cpp


  common/tick-aligned-loop.cpp
  container/infrared-config.cpp
  container/infrared-cursor.cpp

  functionals/uinput-helper.cpp

  emulation/event-device.cpp
  emulation/input-device.cpp

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

  functionals/uinput-helper.h

  common/tick-aligned-loop.h
  container/infrared-config.h
  container/infrared-cursor.h
  emulation/event-device.h
  emulation/input-device.h

  helper/hashcompare.h
)

set(headers_wrap
  ${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/deviceevents.h
  
  dbus/customjobs.h
  dbus/profilemanager.h
  dbus/uinputservice.h

  eiojoystick/eioclassicjoystick.h
  eiojoystick/eionunchukjoystick.h
  eiojoystick/eiowiimotejoystick.h

  eiomanager/manager.h
  eiokeyboard/eioremotekeyboard.h
  eioinfrared/eioinfraredmouse.h
)

add_executable(wiimotedev-io ${headers_wrap} ${headers} ${sources})

target_link_libraries(wiimotedev-io
  Qt5::Core
  Qt5::DBus
  Qt5::Network
  Qt5::Widgets
)

install(TARGETS wiimotedev-io DESTINATION ${WIIMOTEDEV_SBIN_DIR})
