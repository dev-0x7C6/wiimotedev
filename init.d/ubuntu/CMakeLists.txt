# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

set(CURRENT "ubuntu")
set(INSTALL "/etc/init.d/")

STRING(COMPARE EQUAL "${DISTRO}" "${CURRENT}" valid)

if (valid) 
  if (BUILD_DAEMON)
    message( "-- Init.d script for ${DISTRO} is available [wiimotedev-daemon]" )
    install(FILES wiimotedev-daemon DESTINATION "${INSTALL}"
	      PERMISSIONS
		OWNER_READ  OWNER_WRITE  OWNER_EXECUTE
		GROUP_READ
		WORLD_READ
    )
  endif (BUILD_DAEMON)

  if (BUILD_IO)
    message( "-- Init.d script for ${DISTRO} is available [wiimotedev-io]" )
    install(FILES wiimotedev-io DESTINATION "${INSTALL}"
	      PERMISSIONS
		OWNER_READ  OWNER_WRITE  OWNER_EXECUTE
		GROUP_READ
		WORLD_READ
    )
  endif (BUILD_IO)
  
endif (valid)
