include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/linux/usr/include/
)

set(sources
	api/cwiid-controller.cpp
	api/cwiid-controller.h
	api/cwiid-manager.cpp
	api/cwiid-manager.h
	api/xwiimote-controller.cpp
	api/xwiimote-controller.h
	api/xwiimote-manager.cpp
	api/xwiimote-manager.h
	api/xwiimote-monitor.cpp
	api/xwiimote-monitor.h
	${CMAKE_SOURCE_DIR}/config.h
	${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/consts.h
	${CMAKE_SOURCE_DIR}/syslog/syslog.cpp
	${CMAKE_SOURCE_DIR}/syslog/syslog.h
	daemon.cpp
	dbus/daemonservice.h
	dbus/deviceevents.h
	fancy/fancy.cpp
	interfaces/iwiimote-api.cpp
	interfaces/iwiimote-api.h
	interfaces/iwiimote-manager.cpp
	interfaces/iwiimote-manager.h
	messages/classicmessages.cpp
	messages/motionplusmessages.cpp
	messages/nunchukmessages.cpp
	messages/wiimotemessages.cpp
	virtualcursor/virtualcursor.cpp
	virtualcursor/virtualcursor.h
	wiimotedevconnection.cpp
	wiimotedevconnection.h
	wiimotedevcore.cpp
	wiimotedevcore.h
	wiimotedevdevice.cpp
	wiimotedevdevice.h
	wiimotedevsettings.cpp
	wiimotedevsettings.h

)

add_executable(wiimotedev-daemon ${sources} ${headers} ${wiimotedev_project_files})

target_link_libraries(wiimotedev-daemon
	${BLUETOOTH_LIBRARIES}
	Qt5::Core
	Qt5::DBus
	Qt5::Network
	libcwiid
	xwiimote
)

install(TARGETS wiimotedev-daemon DESTINATION ${WIIMOTEDEV_SBIN_DIR})
