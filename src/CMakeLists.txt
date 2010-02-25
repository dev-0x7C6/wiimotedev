set(QT_DONT_USE_QTGUI TRUE)
set(QT_USE_QTDBUS TRUE)

include( ${QT_USE_FILE} )

add_definitions("-DDAEMON_SUPPORT -DSYSLOG_SUPPORT -Wall -ansi")

set (wiimotedev_sources
        main.cpp
        connectionmanager.cpp
        wiimoteconnection.cpp
)

set (wiimotedev_headers
        connectionmanager.h
        wiimoteconnection.h
        wiimotedev.h
)

QT4_WRAP_CPP(wiimotedev_headers ${wiimotedev_headers} )

add_executable(wiimotedev ${wiimotedev_sources} ${wiimotedev_headers})

target_link_libraries(wiimotedev ${QT_LIBRARIES} ${CWIID_LIBRARY})

install(FILES wiimotedev.h DESTINATION ${SERVICE_HEADER_DIR} )
install(TARGETS wiimotedev DESTINATION ${SERVICE_BIN_DIR} )
