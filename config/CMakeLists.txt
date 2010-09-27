# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

if(BUILD_DAEMON) 
  if( NOT EXISTS  "/etc/wiimotedev/wiimotedev.conf" ) 
    install(FILES wiimotedev.conf DESTINATION /etc/wiimotedev/ )
  endif()
endif(BUILD_DAEMON)

if(BUILD_CLIENT)
  if( NOT EXISTS  "/etc/wiimotedev/client.conf" )
    install(FILES client.conf DESTINATION /etc/wiimotedev/ )
  endif()
endif(BUILD_CLIENT)

if(BUILD_UINPUT)
  if( NOT EXISTS  "/etc/wiimotedev/scancode.conf" )
    install(FILES scancode.conf DESTINATION /etc/wiimotedev/ )
  endif()
endif(BUILD_UINPUT)
