set(QT_DONT_USE_QTGUI TRUE)
set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTNETWORK TRUE)

include( ${QT_USE_FILE} )

add_definitions("-DDAEMON_SUPPORT -DSYSLOG_SUPPORT -Wall -ansi")

set (wiimotedev_sources
        main.cpp
        wiimoteconnection.cpp
        messageserver.cpp
        connectionmanager.cpp
)

set (wiimotedev_headers_qt
        wiimoteconnection.h
        messageserver.h
        connectionmanager.h
)

set (wiimotedev_headers
        syslogsupport.h
)

QT4_WRAP_CPP(wiimotedev_headers_qt ${wiimotedev_headers_qt} )

add_executable(wiimotedev-daemon ${wiimotedev_sources} ${wiimotedev_headers} ${wiimotedev_headers_qt})

target_link_libraries(wiimotedev-daemon ${QT_LIBRARIES} ${CWIID_LIBRARY})

install(TARGETS wiimotedev-daemon DESTINATION /usr/sbin/)
install(FILES wiimotedev.h DESTINATION /usr/include/)
install(FILES wiimotedevproto.h DESTINATION /usr/include/)
