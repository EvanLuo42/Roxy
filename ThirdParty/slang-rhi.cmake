set(SLANG_RHI_BUILD_TESTS OFF)
set(SLANG_RHI_BUILD_EXAMPLES OFF)
set(SLANG_RHI_INSTALL OFF)

set(SLANG_RHI_ENABLE_WGPU OFF)

roxy_add_dependency(
        slang-rhi
        GIT_REPOSITORY https://github.com/shader-slang/slang-rhi.git
        GIT_TAG 29dc332e55d8b6afa2e532cbf07d98d34bd9b029
)

add_executable(slangc IMPORTED GLOBAL)
set_target_properties(slangc PROPERTIES
        IMPORTED_LOCATION "${SLANG_RHI_SLANG_BINARY_DIR}/bin/slangc${CMAKE_EXECUTABLE_SUFFIX}"
)
