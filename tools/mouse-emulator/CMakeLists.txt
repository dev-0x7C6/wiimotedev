include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_SOURCE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(sources ir.cpp ir.hpp main.cpp)

qt6_add_dbus_interface(
  sources
  ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.virtualcursor.xml
  dbus/interfaces/virtualcursor)

add_executable(wiimotedev-mouse-emu ${sources})

target_link_libraries(wiimotedev-mouse-emu Qt6::Core Qt6::Gui Qt6::DBus
                      Qt6::Qml Qt6::Quick)

install(TARGETS wiimotedev-mouse-emu)
