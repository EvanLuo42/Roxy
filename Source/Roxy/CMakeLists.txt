add_library(RoxyRuntime STATIC
        Roxy.h
        Core/Application.cpp
        Core/Application.h
        Core/Window.cpp
        Core/Window.h
        Core/Version.cpp
        Core/Version.h
)

target_include_directories(RoxyRuntime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(RoxyRuntime
        PUBLIC slang-rhi slang glfw
)

add_library(Roxy::Runtime ALIAS RoxyRuntime)
