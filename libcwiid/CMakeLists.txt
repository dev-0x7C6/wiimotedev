cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}
                    ${QT_INCLUDE_DIR}
)


set(libcwiid_sources
  bluetooth.c
  command.c
  connect.c
  cwiid.h
  cwiid_internal.h
  interface.c
  process.c
  state.c
  thread.c
  util.c
)

ADD_LIBRARY(libcwiid STATIC
  ${libcwiid_sources}
)
