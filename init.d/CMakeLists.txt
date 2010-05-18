# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

set(init_script "/etc/init.d")

if( EXISTS  "/etc/rc.d" AND IS_DIRECTORY "/etc/rc.d") 
  set(init_script "/etc/rc.d/")
endif( EXISTS  "/etc/rc.d" AND IS_DIRECTORY "/etc/rc.d")

if( EXISTS  "/etc/init.d" AND IS_DIRECTORY "/etc/init.d") 
  set(init_script "/etc/init.d/")
endif( EXISTS  "/etc/init.d" AND IS_DIRECTORY "/etc/init.d")


set( service "init.d/${DISTRO}/wiimotedev-daemon")
set( install_path "${DISTRO}/wiimotedev-daemon" )

if( EXISTS ${service})
  message( "-- Init.d script for ${DISTRO} is present" )
  install(FILES ${install_path} DESTINATION ${init_script}
	  PERMISSIONS
	    OWNER_READ  OWNER_WRITE  OWNER_EXECUTE
	    GROUP_READ  GROUP_EXECUTE
	    WORLD_READ  WORLD_EXECUTE
  )
endif( EXISTS ${service} )
