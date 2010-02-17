set(QT_DONT_USE_QTGUI TRUE)
set(QT_USE_QTDBUS TRUE)

include( ${QT_USE_FILE} )

set(wiimotedev_sources
       main.cpp
       connectionmanager.cpp
       wiimoteconnection.cpp
)

qt4_automoc(${wiimotedev_sources})

add_executable(wiimotedev ${wiimotedev_sources})

target_link_libraries(wiimotedev ${QT_LIBRARIES} ${CWIID_LIBRARY})