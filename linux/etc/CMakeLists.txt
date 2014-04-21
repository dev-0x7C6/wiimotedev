# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2008-2014 Bartłomiej Burdukiewicz
# Contact: bartlomiej.burdukiewicz@gmail.com

add_subdirectory(dbus-1)
if(INSTALL_INITD)
  add_subdirectory(init.d)
endif()
add_subdirectory(wiimotedev)
