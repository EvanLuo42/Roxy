file(GLOB ROXY_DEPENDENCIES_CMAKE
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cmake"
)

foreach (CMAKE_FILE ${ROXY_DEPENDENCIES_CMAKE})
    include(${CMAKE_FILE})
endforeach ()