include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
)

set(sources
	${CMAKE_SOURCE_DIR}/config.h
	${CMAKE_SOURCE_DIR}/include/wiimotedev/wiimotedev
	${CMAKE_SOURCE_DIR}/common/enums/device.cpp
	${CMAKE_SOURCE_DIR}/common/enums/device.h
	common/tick-aligned-loop.cpp
	common/tick-aligned-loop.h
	container/infrared-config.cpp
	container/infrared-config.h
	container/infrared-cursor.cpp
	container/infrared-cursor.h
	container/stick-container.cpp
	container/stick-container.h
	core/profile-manager.cpp
	core/profile-manager.h
	eioinfrared/eioinfraredmouse.cpp
	eioinfrared/eioinfraredmouse.h
	eiokeyboard/eioremotekeyboard.cpp
	eiokeyboard/eioremotekeyboard.h
	eiomanager/actions.cpp
	eiomanager/infrared.cpp
	eiomanager/keyboard.cpp
	eiomanager/manager.cpp
	eiomanager/manager.h
	emulation/event-device.cpp
	emulation/event-device.h
	emulation/gamepads/balance-board-gamepad.cpp
	emulation/gamepads/balance-board-gamepad.h
	emulation/gamepads/classic-gamepad.cpp
	emulation/gamepads/classic-gamepad.h
	emulation/gamepads/nunchuk-gamepad.cpp
	emulation/gamepads/nunchuk-gamepad.h
	emulation/gamepads/pro-controller-gamepad.cpp
	emulation/gamepads/pro-controller-gamepad.h
	emulation/gamepads/wiimote-gamepad.cpp
	emulation/gamepads/wiimote-gamepad.h
	emulation/input-device.cpp
	emulation/input-device.h
	factories/gamepad-factory.cpp
	factories/gamepad-factory.h
	functionals/balance-board-processor.cpp
	functionals/balance-board-processor.h
	functionals/hash-compare.h
	functionals/profile.cpp
	functionals/profile.h
	functionals/uinput-helper.cpp
	functionals/uinput-helper.h
	interfaces/igamepad.cpp
	interfaces/igamepad.h
	interfaces/iinput-device.cpp
	interfaces/iinput-device.h
	interfaces/imouse.cpp
	interfaces/imouse.h
	interfaces/iprofile.cpp
	interfaces/iprofile.h
	interfaces/iwiimotedev-event.cpp
	interfaces/iwiimotedev-event.h
	main.cpp
)

qt5_add_dbus_adaptor(sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.profilemanager.xml core/profile-manager.h io::core::ProfileManager)
qt5_add_dbus_interface(sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.balanceboard.xml dbus/interfaces/balanceboard)
qt5_add_dbus_interface(sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.classic.xml dbus/interfaces/nunchuk)
qt5_add_dbus_interface(sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.nunchuk.xml dbus/interfaces/classic)
qt5_add_dbus_interface(sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.procontroller.xml dbus/interfaces/procontroller)
qt5_add_dbus_interface(sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.wiimote.xml dbus/interfaces/wiimote)
qt5_add_dbus_interface(sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.virtualcursor.xml dbus/interfaces/virtualcursor)

add_executable(wiimotedev-io ${sources})

target_link_libraries(
	wiimotedev-io
	Qt5::Core
	Qt5::DBus
	Qt5::Widgets
)

install(TARGETS wiimotedev-io
	DESTINATION "${CMAKE_INSTALL_PREFIX}/sbin/"
	PERMISSIONS
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE
)
