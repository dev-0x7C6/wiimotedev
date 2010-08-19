# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

set(CURRENT "slackware")

STRING(COMPARE EQUAL "${DISTRO}" "${CURRENT}" valid)

if (valid) 
  message( "-- Init.d script for ${DISTRO} is available" )
  install(FILES wiimotedev-daemon DESTINATION ${init_directory}
	    PERMISSIONS
	      OWNER_READ  OWNER_WRITE  OWNER_EXECUTE
	      GROUP_READ  GROUP_EXECUTE
	      WORLD_READ  WORLD_EXECUTE
  )
endif (valid)
