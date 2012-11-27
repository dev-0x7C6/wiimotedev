# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

if(BUILD_DAEMON)
  install(FILES org.wiimotedev-daemon.conf DESTINATION ${WIIMOTEDEV_DBUS_DIR})
endif(BUILD_DAEMON)

if(BUILD_UINPUT)
  install(FILES org.wiimotedev-uinput.conf DESTINATION ${WIIMOTEDEV_DBUS_DIR})
endif(BUILD_UINPUT)
