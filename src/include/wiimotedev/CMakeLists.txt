# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

set (wiimotedev_includes
	consts.h
	proto.h
	interface.h
	readme
)

install(FILES ${wiimotedev_includes} DESTINATION ${include_directory}/wiimotedev)

# compatibility with wiimotedev-daemon 1.0

install(FILES consts.h DESTINATION ${include_directory}/wiimotedev.h)
install(FILES proto.h DESTINATION ${include_directory}/wiimotedevproto.h)