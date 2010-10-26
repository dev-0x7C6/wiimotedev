# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

if(BUILD_QWIIMOTEDEV)
  add_subdirectory(qwiimotedev)
endif(BUILD_QWIIMOTEDEV)

if(BUILD_REGISTER)
  add_subdirectory(register)
endif(BUILD_REGISTER)

if(BUILD_TOOLKIT)
  add_subdirectory(toolkit)
endif(BUILD_TOOLKIT)
