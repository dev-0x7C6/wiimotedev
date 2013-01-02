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

add_library(libcwiid STATIC
  ${libcwiid_sources}
)

target_link_libraries(libcwiid
  ${BLUETOOTH_LIBRARIES}
  "-lpthread"
  "-lrt"
)
