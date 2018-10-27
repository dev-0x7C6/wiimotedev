set(dbus-configs
	org.wiimotedev.daemon.conf
	org.wiimotedev.io.conf
)

install(FILES ${dbus-configs}
	DESTINATION "/etc/dbus-1/system.d/"
	PERMISSIONS
		OWNER_READ OWNER_WRITE
		GROUP_READ
		WORLD_READ
)
