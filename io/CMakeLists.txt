include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/linux/usr/include/
)

set(sources
	${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/consts.h
	${CMAKE_SOURCE_DIR}/linux/usr/include/wiimotedev/deviceevents.h
	${CMAKE_SOURCE_DIR}/syslog/syslog.cpp
	${CMAKE_SOURCE_DIR}/syslog/syslog.h
	common/tick-aligned-loop.cpp
	common/tick-aligned-loop.h
	container/infrared-config.cpp
	container/infrared-config.h
	container/infrared-cursor.cpp
	container/infrared-cursor.h
	container/stick-container.cpp
	container/stick-container.h
	dbus/customjobs.h
	dbus/profilemanager.h
	dbus/uinputservice.h
	eioinfrared/eioinfraredmouse.cpp
	eioinfrared/eioinfraredmouse.h
	eiokeyboard/eioremotekeyboard.cpp
	eiokeyboard/eioremotekeyboard.h
	eiomanager/actions.cpp
	eiomanager/eiojoystickassign.cpp
	eiomanager/infrared.cpp
	eiomanager/keyboard.cpp
	eiomanager/manager.cpp
	eiomanager/manager.h
	emulation/event-device.cpp
	emulation/event-device.h
	emulation/gamepads/classic-gamepad.cpp
	emulation/gamepads/classic-gamepad.h
	emulation/gamepads/nunchuk-gamepad.cpp
	emulation/gamepads/nunchuk-gamepad.h
	emulation/gamepads/wiimote-gamepad.cpp
	emulation/gamepads/wiimote-gamepad.h
	emulation/iinput-device.cpp
	emulation/iinput-device.cpp
	emulation/input-device.cpp
	emulation/input-device.h
	factories/gamepad-factory.cpp
	factories/gamepad-factory.h
	functionals/hash-compare.h
	functionals/uinput-helper.cpp
	functionals/uinput-helper.h
	interfaces/igamepad.cpp
	interfaces/igamepad.h
	uinput.cpp
)

add_executable(wiimotedev-io ${sources})

target_link_libraries(
	wiimotedev-io
	Qt5::Core
	Qt5::DBus
	Qt5::Network
	Qt5::Widgets
)

install(TARGETS wiimotedev-io DESTINATION ${WIIMOTEDEV_SBIN_DIR})
