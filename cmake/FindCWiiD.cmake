# Distributed under the terms of the GNU Lesser General Public License, v2.1 or later
# Copyright (C) 2010 Bartłomiej Burdukiewicz
# Contact: dev.strikeu@gmail.com

cmake_minimum_required(VERSION 2.6)

find_path(CWIID_INCLUDE_DIR cwiid.h)
find_library(CWIID_LIBRARY cwiid)

if (CWIID_INCLUDE_DIR AND CWIID_LIBRARY)
	if (NOT CWiiD_FIND_QUIETLY)
		message(STATUS "Located libcwiid includes at ${CWIID_INCLUDE_PATH} and library at ${CWIID_LIBRARY}")
	endif (NOT CWiiD_FIND_QUIETLY)
else (CWIID_INCLUDE_DIR AND CWIID_LIBRARY)
	if (CWiiD_FIND_REQUIRED)
		message(FATAL_ERROR
			"Could not find libcwiid. Install with "
			"sudo apt-get install libcwiid1-dev"
		)
	endif (CWiiD_FIND_REQUIRED)
endif (CWIID_INCLUDE_DIR AND CWIID_LIBRARY)

