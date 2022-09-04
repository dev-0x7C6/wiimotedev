include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_SOURCE_DIR})

set(sources
    ${CMAKE_SOURCE_DIR}/common/enums/device.cpp
    ${CMAKE_SOURCE_DIR}/common/enums/device.h
    ${CMAKE_CURRENT_BINARY_DIR}/../config.h
    ${CMAKE_SOURCE_DIR}/include/wiimotedev/wiimotedev
    api/xwiimote-controller.cpp
    api/xwiimote-controller.h
    api/xwiimote-manager.cpp
    api/xwiimote-manager.h
    api/xwiimote-monitor.cpp
    api/xwiimote-monitor.h
    containers/structs.hpp
    controllers/wiimote-id-manager.cpp
    controllers/wiimote-id-manager.h
    containers/structs.hpp
    controllers/wiimote-scanner-thread.cpp
    controllers/wiimote-scanner-thread.h
    controllers/wiimote-scanner.cpp
    controllers/wiimote-scanner.h
    dbus/balance-board-dispatcher.cpp
    dbus/balance-board-dispatcher.h
    dbus/classic-dispatcher.cpp
    dbus/classic-dispatcher.h
    dbus/devices-dispatcher.cpp
    dbus/devices-dispatcher.h
    dbus/nunchuk-dispatcher.cpp
    dbus/nunchuk-dispatcher.h
    dbus/pro-controller-dispatcher.cpp
    dbus/pro-controller-dispatcher.h
    dbus/virtual-cursor-dispatcher.cpp
    dbus/virtual-cursor-dispatcher.h
    dbus/wiimote-dispatcher.cpp
    dbus/wiimote-dispatcher.h
    enums/adaptor.h
    enums/event.h
    factories/controller-manager-factory.cpp
    factories/controller-manager-factory.h
    factories/dispatcher-factory.cpp
    factories/dispatcher-factory.h
    functionals/unique-id.h
    functionals/virtual-cursor-processor.cpp
    functionals/virtual-cursor-processor.h
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
    wiimotedevcore.h)

qt5_add_dbus_adaptor(
  sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.balanceboard.xml
  dbus/balance-board-dispatcher.h dae::dbus::BalanceBoardDispatcher)
qt5_add_dbus_adaptor(
  sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.classic.xml
  dbus/classic-dispatcher.h dae::dbus::ClassicDispatcher)
qt5_add_dbus_adaptor(
  sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.nunchuk.xml
  dbus/nunchuk-dispatcher.h dae::dbus::NunchukDispatcher)
qt5_add_dbus_adaptor(
  sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.devices.xml
  dbus/devices-dispatcher.h dae::dbus::DevicesDispatcher)
qt5_add_dbus_adaptor(
  sources
  ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.procontroller.xml
  dbus/pro-controller-dispatcher.h dae::dbus::ProControllerDispatcher)
qt5_add_dbus_adaptor(
  sources
  ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.virtualcursor.xml
  dbus/virtual-cursor-dispatcher.h dae::dbus::VirtualCursorDispatcher)
qt5_add_dbus_adaptor(
  sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.wiimote.xml
  dbus/wiimote-dispatcher.h dae::dbus::WiimoteDispatcher)

add_executable(wiimotedev-daemon ${sources} ${headers}
                                 ${wiimotedev_project_files})

target_link_libraries(
  wiimotedev-daemon
  ${BLUETOOTH_LIBRARIES}
  Qt5::Core
  Qt5::DBus
  spdlog::spdlog
  Eigen3::Eigen
  xwiimote-ng
  pthread)

install(
  TARGETS wiimotedev-daemon
  DESTINATION "${CMAKE_INSTALL_PREFIX}/sbin/"
  PERMISSIONS
    OWNER_READ
    OWNER_WRITE
    OWNER_EXECUTE
    GROUP_READ
    GROUP_EXECUTE
    WORLD_READ
    WORLD_EXECUTE)
