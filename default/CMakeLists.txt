IF( NOT EXISTS  "${SERVICE_CONF_DIR}wiimotedev.conf" ) 
    install(FILES wiimotedev.conf DESTINATION ${SERVICE_CONF_DIR} )
ENDIF()

