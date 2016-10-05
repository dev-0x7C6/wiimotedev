include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}
)

set(sources
	api/xwiimote-controller.cpp
	api/xwiimote-controller.h
	api/xwiimote-manager.cpp
	api/xwiimote-manager.h
	api/xwiimote-monitor.cpp
	api/xwiimote-monitor.h
	${CMAKE_SOURCE_DIR}/config.h
	${CMAKE_SOURCE_DIR}/include/wiimotedev/wiimotedev
	${CMAKE_SOURCE_DIR}/common/enums/device.cpp
	${CMAKE_SOURCE_DIR}/common/enums/device.h
	containers/accelerometer-container.cpp
	containers/accelerometer-container.h
	containers/button-container.cpp
	containers/button-container.h
	containers/gyroscope-container.cpp
	containers/gyroscope-container.h
	containers/infrared-container.cpp
	containers/infrared-container.h
	containers/pressure-container.cpp
	containers/pressure-container.h
	containers/status-container.cpp
	containers/status-container.h
	containers/stick-container.cpp
	containers/stick-container.h
	controllers/wiimote-id-manager.cpp
	controllers/wiimote-id-manager.h
	controllers/wiimote-scanner.cpp
	controllers/wiimote-scanner.h
	controllers/wiimote-scanner-thread.cpp
	controllers/wiimote-scanner-thread.h
	dbus/balance-board-dispatcher.cpp
	dbus/balance-board-dispatcher.h
	dbus/classic-dispatcher.cpp
	dbus/classic-dispatcher.h
	dbus/nunchuk-dispatcher.cpp
	dbus/nunchuk-dispatcher.h
	dbus/pro-controller-dispatcher.cpp
	dbus/pro-controller-dispatcher.h
	dbus/wiimote-dispatcher.cpp
	dbus/wiimote-dispatcher.h
	enums/event.cpp
	enums/event.h
	factories/controller-manager-factory.cpp
	factories/controller-manager-factory.h
	factories/dispatcher-factory.cpp
	factories/dispatcher-factory.h
	functionals/unique-id.h
	interfaces/icontainer.h
	interfaces/icontainer-processor.cpp
	interfaces/icontainer-processor.h
	interfaces/iid-manager.cpp
	interfaces/iid-manager.h
	interfaces/iwiimote-api.cpp
	interfaces/iwiimote-api.h
	interfaces/iwiimote-manager.cpp
	interfaces/iwiimote-manager.h
	main.cpp
	wiimotedevcore.cpp
	wiimotedevcore.h
)

#qt5_add_dbus_adaptor(sources dbus/deviceevents.xml wiimotedevcore.h service::core::WiimotedevCore)
qt5_add_dbus_adaptor(sources ../dbus/balanceboard.xml dbus/balance-board-dispatcher.h dae::dbus::BalanceBoardDispatcher)
qt5_add_dbus_adaptor(sources ../dbus/classic.xml dbus/classic-dispatcher.h dae::dbus::ClassicDispatcher)
qt5_add_dbus_adaptor(sources ../dbus/nunchuk.xml dbus/nunchuk-dispatcher.h dae::dbus::NunchukDispatcher)
qt5_add_dbus_adaptor(sources ../dbus/wiimote.xml dbus/wiimote-dispatcher.h dae::dbus::WiimoteDispatcher)
qt5_add_dbus_adaptor(sources ../dbus/motionplus.xml wiimotedevcore.h dae::core::WiimotedevCore)
qt5_add_dbus_adaptor(sources ../dbus/procontroller.xml dbus/pro-controller-dispatcher.h dae::dbus::ProControllerDispatcher)

add_executable(wiimotedev-daemon ${sources} ${headers} ${wiimotedev_project_files})

target_link_libraries(wiimotedev-daemon
	${BLUETOOTH_LIBRARIES}
	Qt5::Core
	Qt5::DBus
	xwiimote
	pthread
)

install(TARGETS wiimotedev-daemon DESTINATION ${WIIMOTEDEV_SBIN_DIR})
