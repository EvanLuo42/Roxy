add_library(RoxyRuntime STATIC
        Roxy.h
        Core/Application.cpp
        Core/Application.h
        Core/Window.cpp
        Core/Window.h
        Core/Version.cpp
        Core/Version.h
)

target_link_libraries(RoxyRuntime PRIVATE slang slang-rhi glfw)

add_library(Roxy::Runtime ALIAS RoxyRuntime)
