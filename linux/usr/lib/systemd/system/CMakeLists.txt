# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2016 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

set(services
  wiimotedev-daemon.service
  wiimotedev-io.service
)

install(FILES ${services} DESTINATION "${WIIMOTEDEV_SYSD_DIR}"
    PERMISSIONS
    OWNER_READ  OWNER_WRITE
    GROUP_READ
    WORLD_READ
)
