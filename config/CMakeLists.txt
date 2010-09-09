# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

IF( NOT EXISTS  "/etc/wiimotedev/wiimotedev.conf" ) 
    install(FILES wiimotedev.conf DESTINATION /etc/wiimotedev/ )
ENDIF()

IF( NOT EXISTS  "/etc/wiimotedev/client.conf" )
    install(FILES client.conf DESTINATION /etc/wiimotedev/ )
ENDIF()

