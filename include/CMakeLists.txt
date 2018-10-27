set(system-headers
	wiimotedev/wiimotedev
)

install(FILES ${system-headers} 
	DESTINATION "${CMAKE_INSTALL_PREFIX}/include/wiimotedev/"
	PERMISSIONS
		OWNER_READ OWNER_WRITE
		GROUP_READ
		WORLD_READ
)
