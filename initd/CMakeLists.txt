set( service "initd/${DISTRO}/wiimotedev-daemon")
set( install_path "${DISTRO}/wiimotedev-daemon" )

if( EXISTS ${service})
  message( "Init.d script for ${DISTRO} is present" )
  install(FILES ${install_path} DESTINATION ${SERVICE_INITD_DIR}
	  PERMISSIONS
	    OWNER_READ  OWNER_WRITE  OWNER_EXECUTE
	    GROUP_READ  GROUP_EXECUTE
	    WORLD_READ  WORLD_EXECUTE
  )
endif( EXISTS ${service} )