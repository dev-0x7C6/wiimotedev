# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

set(FILELIST
  scancode.conf
  wiimotedev.conf
)

install(FILES ${FILELIST} DESTINATION ${WIIMOTEDEV_CONF_DIR})
