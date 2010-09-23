# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

set (wiimotedev_headers
  consts.h
  proto.h
  interface.h
)

install(FILES ${wiimotedev_headers} DESTINATION ${include_directory}/wiimotedev)