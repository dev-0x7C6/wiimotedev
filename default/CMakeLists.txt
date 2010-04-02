IF( NOT EXISTS  "/etc/wiimotedev/wiimotedev.conf" ) 
    install(FILES wiimotedev.conf DESTINATION /etc/wiimotedev/ )
ENDIF()

