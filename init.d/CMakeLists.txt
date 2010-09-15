# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

if(BUILD_UINPUT)
  add_subdirectory(uinput)
endif(BUILD_UINPUT)

if(BUILD_DAEMON) 
  add_subdirectory(daemon)
endif(BUILD_DAEMON)

if(BUILD_CLIENT)
  add_subdirectory(client)
endif(BUILD_CLIENT)
