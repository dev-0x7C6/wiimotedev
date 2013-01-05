# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

set (HEADERS
  consts.h
  deviceevents.h
)

install(FILES ${HEADERS} DESTINATION ${WIIMOTEDEV_INC_DIR}/wiimotedev)
