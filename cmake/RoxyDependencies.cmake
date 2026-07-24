include(FetchContent)

function(roxy_add_dependency NAME)
    cmake_parse_arguments(
        ARG
        ""
        "GIT_REPOSITORY;GIT_TAG"
        ""
        ${ARGN}
    )

    if (NOT ARG_GIT_REPOSITORY)
        message(FATAL_ERROR
            "roxy_add_dependency(${NAME}): GIT_REPOSITORY is required"
        )
    endif()

    if(NOT ARG_GIT_TAG)
        set(ARG_GIT_TAG main)
    endif()

    message(STATUS "Fetching dependency ${NAME}")
    message(STATUS "  Repository: ${ARG_GIT_REPOSITORY}")
    message(STATUS "  Tag: ${ARG_GIT_TAG}")

    FetchContent_GetProperties(${NAME})

    if(NOT ${NAME}_POPULATED)
        FetchContent_Declare(
            ${NAME}
            GIT_REPOSITORY ${ARG_GIT_REPOSITORY}
            GIT_TAG        ${ARG_GIT_TAG}
            GIT_SHALLOW    TRUE
            EXCLUDE_FROM_ALL TRUE
        )

        FetchContent_MakeAvailable(${NAME})
    else()
        message(STATUS "${NAME} already fetched")
    endif()
endfunction()