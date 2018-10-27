set(systemd-services
	wiimotedev-daemon.service
	wiimotedev-io.service
)

install(FILES ${systemd-services} 
	DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/systemd/system/"
	PERMISSIONS
		OWNER_READ OWNER_WRITE
		GROUP_READ
		WORLD_READ
)
