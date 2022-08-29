include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_SOURCE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(sources ir.cpp ir.hpp main.cpp qml.qrc)

qt5_add_dbus_interface(
  sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.wiimote.xml
  dbus/interfaces/wiimote)
qt5_add_dbus_interface(
  sources
  ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.virtualcursor.xml
  dbus/interfaces/virtualcursor)

add_executable(wiimotedev-ir-monitor ${sources})

target_link_libraries(wiimotedev-ir-monitor Qt5::Core Qt5::Gui Qt5::DBus
                      Qt5::Qml Qt5::Quick)

install(TARGETS wiimotedev-ir-monitor)
