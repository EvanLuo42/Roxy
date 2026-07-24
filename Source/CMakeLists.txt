add_subdirectory(RenderGraphs)
add_subdirectory(RenderPasses)
add_subdirectory(Roxy)
if (ROXY_BUILD_TESTS)
    add_subdirectory(Tests)
endif ()
add_subdirectory(Viewer)

set(ROXY_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ROXY_SHADER_OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders)

file(GLOB_RECURSE ROXY_SHADER_SOURCES CONFIGURE_DEPENDS
        ${ROXY_SHADER_DIR}/*.slang
        ${ROXY_SHADER_DIR}/*.slangh
)

set(ROXY_SHADER_MODULES)
foreach (SHADER_SOURCE ${ROXY_SHADER_SOURCES})
    file(RELATIVE_PATH SHADER_REL ${ROXY_SHADER_DIR} ${SHADER_SOURCE})
    string(REGEX REPLACE "\\.(slang|slangh)$" ".slang-module" SHADER_MODULE ${SHADER_REL})
    set(SHADER_OUT ${ROXY_SHADER_OUT_DIR}/${SHADER_MODULE})
    get_filename_component(SHADER_OUT_PARENT ${SHADER_OUT} DIRECTORY)

    add_custom_command(
            OUTPUT ${SHADER_OUT}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_OUT_PARENT}
            COMMAND $<TARGET_FILE:slangc> -lang slang -I ${ROXY_SHADER_DIR} ${SHADER_SOURCE} -o ${SHADER_OUT}
            DEPENDS ${SHADER_SOURCE} slangc
            COMMENT "Compiling shader ${SHADER_REL}"
            VERBATIM
    )
    list(APPEND ROXY_SHADER_MODULES ${SHADER_OUT})
endforeach ()

add_custom_target(RoxyShaders ALL DEPENDS ${ROXY_SHADER_MODULES})
add_dependencies(RoxyRuntime RoxyShaders)

target_compile_definitions(RoxyRuntime PRIVATE ROXY_SHADER_DIR=\"${ROXY_SHADER_OUT_DIR}\")
