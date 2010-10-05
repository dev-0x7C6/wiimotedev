# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

set(CURRENT "debian")

STRING(COMPARE EQUAL "${DISTRO}" "${CURRENT}" valid)

if (valid) 
  message( "-- Init.d script for ${DISTRO} is available [wiimotedev-uinput]" )
  install(FILES wiimotedev-uinput DESTINATION /etc/init.d
	    PERMISSIONS
	      OWNER_READ  OWNER_WRITE  OWNER_EXECUTE
	      GROUP_READ
	      WORLD_READ
  )
endif (valid)
