# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

# build static libcwiid
if(USE_STATIC_CWIID)
  add_subdirectory(libcwiid)
endif(USE_STATIC_CWIID)

add_subdirectory(qtsingleapplication)
