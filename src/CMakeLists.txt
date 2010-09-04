# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wall -Wno-sign-compare ${CMAKE_CXX_FLAGS}")

configure_file(config.h.in
               config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(include)

set(SBIN_INSTALL_DIR "${EXEC_INSTALL_PREFIX}/sbin")

set (wiimotedev_sources
  dbus/support.cpp
  dbus/deviceevents.cpp
  dbus/service.cpp
  network/support.cpp
  wiimotedev/connection.cpp
  wiimotedev/manager.cpp
  wiimotedev/settings.cpp
  wiimotedev/wiiremote.cpp
  daemon.cpp
)

set (wiimotedev_headers_qt
  dbus/deviceevents.h
  dbus/service.h
  network/support.h
  wiimotedev/connection.h
  wiimotedev/manager.h
)

set (wiimotedev_headers
  dbus/support.h
  include/wiimotedev/consts.h
  include/wiimotedev/proto.h
  wiimotedev/settings.h
  wiimotedev/wiiremote.h
)

set (client_sources
  dbus/support.cpp
  dbus/deviceevents.cpp
  dbus/service.cpp
  network/manager.cpp
  client.cpp
)

set (client_headers_qt
  dbus/deviceevents.h
  dbus/service.h
  network/manager.h
)

set (client_headers
  dbus/support.h
  include/wiimotedev/consts.h
  include/wiimotedev/proto.h
)

set (register_sources
  register.cpp
)

set (register_headers
  include/wiimotedev/consts.h
  include/wiimotedev/proto.h
)


set(toolkit_sources
  toolkit.cpp
  toolkit/mainwindow.cpp
  toolkit/selectwiimote.cpp
)

set(toolkit_headers
  include/wiimotedev/consts.h
)

set(toolkit_headers_qt
  toolkit/mainwindow.h
  toolkit/selectwiimote.h
  include/wiimotedev/interface.h
)

set(toolkit_uis
  toolkit/mainwindow.ui
  toolkit/selectwiimote.ui
)

set(car_example_sources
  example-car.cpp
  examples/car/car.cpp
)

set(car_example_headers
  include/wiimotedev/consts.h
)

set(car_example_headers_qt
  examples/car/car.h
  include/wiimotedev/interface.h
)


if(BUILD_DAEMON)
  if(USE_SYSLOG)
    list(APPEND wiimotedev_sources "syslog/syslog.cpp")
    list(APPEND wiimotedev_headers "syslog/syslog.h")
  endif(USE_SYSLOG)

  qt4_wrap_cpp(wiimotedev_headers_qt ${wiimotedev_headers_qt} )

  add_executable(wiimotedev-daemon ${wiimotedev_sources} ${wiimotedev_headers} ${wiimotedev_headers_qt})
  target_link_libraries(wiimotedev-daemon ${QT_LIBRARIES} ${CWIID_LIBRARY})

  install(TARGETS wiimotedev-daemon DESTINATION ${sbin_directory})
endif(BUILD_DAEMON)

if(BUILD_CLIENT)
  if(USE_SYSLOG)
    list(APPEND client_sources "syslog/syslog.cpp")
    list(APPEND client_headers "syslog/syslog.h")
  endif(USE_SYSLOG)

  qt4_wrap_cpp(client_sources ${client_headers_qt} )

  add_executable(wiimotedev-client ${client_sources} ${client_headers})
  target_link_libraries(wiimotedev-client ${QT_LIBRARIES})

  install(TARGETS wiimotedev-client DESTINATION ${sbin_directory})
endif(BUILD_CLIENT)

if(BUILD_REGISTER)
  add_executable(wiimotedev-register ${register_sources} ${register_headers})
  target_link_libraries(wiimotedev-register ${QT_LIBRARIES})

  install(TARGETS wiimotedev-register DESTINATION ${bin_directory})
endif(BUILD_REGISTER)

if(BUILD_TOOLKIT)
  qt4_wrap_ui(toolkit_sources ${toolkit_uis})
  qt4_wrap_cpp(toolkit_sources ${toolkit_headers_qt} )

  add_executable(wiimotedev-toolkit ${toolkit_sources} ${toolkit_headers})
  target_link_libraries(wiimotedev-toolkit ${QT_LIBRARIES})

  install(TARGETS wiimotedev-toolkit DESTINATION ${bin_directory})
endif(BUILD_TOOLKIT)


if(BUILD_EXAMPLES)
  qt4_wrap_cpp(car_example_sources ${car_example_headers_qt} )

  add_executable(wiimotedev-car-example ${car_example_sources} ${car_example_headers})
  target_link_libraries(wiimotedev-car-example ${QT_LIBRARIES})

  install(TARGETS wiimotedev-car-example DESTINATION ${bin_directory})
endif(BUILD_EXAMPLES)
