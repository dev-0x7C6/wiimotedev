# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2014 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

set(services
  wiimotedev-daemon.service
  wiimotedev-daemon.target
  wiimotedev-io.service
  wiimotedev-io.target
)

install(FILES ${services} DESTINATION "${WIIMOTEDEV_SYSD_DIR}"
    PERMISSIONS
    OWNER_READ  OWNER_WRITE  OWNER_EXECUTE
    GROUP_READ
    WORLD_READ
)
