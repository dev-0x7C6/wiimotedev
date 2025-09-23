include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_SOURCE_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(sources
  virtual-cursor-model.cpp
  virtual-cursor-model.hpp
  infrared-model.cpp
  infrared-model.hpp
  main.cpp
  qml.qrc
)

qt6_add_dbus_interface(
  sources ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.wiimote.xml
  dbus/interfaces/wiimote)
qt6_add_dbus_interface(
  sources
  ${CMAKE_SOURCE_DIR}/dbus-1/interfaces/org.wiimotedev.virtualcursor.xml
  dbus/interfaces/virtualcursor)

add_executable(wiimotedev-ir-monitor ${sources})

target_link_libraries(wiimotedev-ir-monitor
  Qt6::Core
  Qt6::Gui
  Qt6::DBus
  Qt6::Qml
  Qt6::Quick
  Qt6::QuickControls2
)

install(TARGETS wiimotedev-ir-monitor)
