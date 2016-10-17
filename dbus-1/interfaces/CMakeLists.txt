set(dbus-interfaces
	org.wiimotedev.balanceboard.xml
	org.wiimotedev.classic.xml
	org.wiimotedev.deviceevents.xml
	org.wiimotedev.nunchuk.xml
	org.wiimotedev.procontroller.xml
	org.wiimotedev.profilemanager.xml
	org.wiimotedev.virtualcursor.xml
	org.wiimotedev.wiimote.xml
)

install(FILES ${dbus-interfaces} 
	DESTINATION "${CMAKE_INSTALL_PREFIX}/share/dbus-1/interfaces/"
	PERMISSIONS
		OWNER_READ  OWNER_WRITE
		GROUP_READ
		WORLD_READ
)
