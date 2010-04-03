set(QT_DONT_USE_QTGUI TRUE)
set(QT_USE_QTDBUS TRUE)
set(QT_USE_QTNETWORK TRUE)

include( ${QT_USE_FILE} )

add_definitions("-DDAEMON_SUPPORT -DSYSLOG_SUPPORT -Wall -ansi")

set (wiimotedev_sources
        main.cpp
        connectionmanager.cpp
        wiimoteconnection.cpp
        messageserver.cpp
        messagethread.cpp
)

set (wiimotedev_headers
        connectionmanager.h
        wiimoteconnection.h
        messageserver.h
        messagethread.h
        wiimotedev.h
        wiimotedevproto.h
)

QT4_WRAP_CPP(wiimotedev_headers ${wiimotedev_headers} )

add_executable(wiimotedev-daemon ${wiimotedev_sources} ${wiimotedev_headers})

target_link_libraries(wiimotedev-daemon ${QT_LIBRARIES} ${CWIID_LIBRARY})

install(FILES wiimotedev.h DESTINATION /usr/include/ )
install(FILES wiimotedevproto.h  DESTINATION /usr/include/ )

install(TARGETS wiimotedev-daemon DESTINATION /usr/sbin/ )
