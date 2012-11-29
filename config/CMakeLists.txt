# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

set(FILELIST
  scancode.conf
  wiimotedev.conf
)

if(BUILD_DAEMON)
  install(FILES org.wiimotedev-daemon.conf DESTINATION ${WIIMOTEDEV_DBUS_DIR})
endif(BUILD_DAEMON)

if(BUILD_IO)
  install(FILES org.wiimotedev-io.conf DESTINATION ${WIIMOTEDEV_DBUS_DIR})
endif(BUILD_IO)


install(FILES ${FILELIST} DESTINATION ${WIIMOTEDEV_CONF_DIR})
