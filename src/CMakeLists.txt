add_subdirectory(rtmessage)
add_subdirectory(core)

if (BUILD_RBUS_DAEMON)
    add_subdirectory(session_manager)
endif (BUILD_RBUS_DAEMON)

add_subdirectory(rbus)
