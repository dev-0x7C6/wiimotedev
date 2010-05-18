set(QT_DONT_USE_QTGUI TRUE)
set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTNETWORK TRUE)

include( ${QT_USE_FILE} )

add_definitions("-DDAEMON_SUPPORT -DSYSLOG_SUPPORT -Wall -ansi")

set (wiimotedev_sources
        dbus/support.cpp
        dbus/deviceevents.cpp
        dbus/service.cpp
        network/support.cpp
        wiimotedev/connection.cpp
        wiimotedev/manager.cpp
        wiimotedev/settings.cpp
        daemon.cpp
)

set (wiimotedev_headers_qt
        dbus/deviceevents.h
        dbus/service.h
        dbus/support.h
        network/support.h
        wiimotedev/connection.h
        wiimotedev/manager.h
        wiimotedev/settings.h
)

set (wiimotedev_headers
        include/wiimotedev.h
        include/wiimotedevproto.h
        syslog/support.h
)

QT4_WRAP_CPP(wiimotedev_headers_qt ${wiimotedev_headers_qt} )

add_executable(wiimotedev-daemon ${wiimotedev_sources} ${wiimotedev_headers} ${wiimotedev_headers_qt})

target_link_libraries(wiimotedev-daemon ${QT_LIBRARIES} ${CWIID_LIBRARY})

install(TARGETS wiimotedev-daemon DESTINATION /usr/sbin/)
install(FILES wiimotedev.h DESTINATION /usr/include/)
install(FILES wiimotedevproto.h DESTINATION /usr/include/)
