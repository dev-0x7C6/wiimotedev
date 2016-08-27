# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

if(WIIMOTEDEV_BUILD_DAEMON)
  install(FILES org.wiimotedev-daemon.conf DESTINATION ${WIIMOTEDEV_DBUS_DIR})
endif(WIIMOTEDEV_BUILD_DAEMON)

if(WIIMOTEDEV_BUILD_IO)
  install(FILES org.wiimotedev-io.conf DESTINATION ${WIIMOTEDEV_DBUS_DIR})
endif(WIIMOTEDEV_BUILD_IO)
