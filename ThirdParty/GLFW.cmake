set(GLFW_BUILD_TESTS FALSE CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES FALSE CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS FALSE CACHE BOOL "" FORCE)

roxy_add_dependency(
        GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)