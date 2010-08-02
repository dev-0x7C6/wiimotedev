# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

set (wiimotedev_includes
	consts.h
	proto.h
	interface.h
	readme
)

install(FILES ${wiimotedev_includes} DESTINATION ${INCLUDE_INSTALL_DIR}/wiimotedev)

# compatibility with wiimotedev-daemon 1.0

install(FILES consts.h DESTINATION ${INCLUDE_INSTALL_DIR}/wiimotedev.h)
install(FILES proto.h DESTINATION ${INCLUDE_INSTALL_DIR}/wiimotedevproto.h)